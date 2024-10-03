#include <stddef.h>
#include <stdint.h>
#include <string.h>

#pragma GCC optimize("O1")

enum { UART_BASE = 0x2000 };

struct uart_csr {
    volatile uint32_t tx_empty : 1;
    volatile uint32_t rx_empty : 1;
    volatile uint32_t reserved0: 30;
} __attribute__((packed, aligned(sizeof(uint32_t))));

struct uart {
    volatile struct uart_csr csr;
    volatile uint32_t tx_space;
    volatile uint32_t rx_space;
} __attribute__((packed, aligned(sizeof(uint32_t))));

#define uart_declare(__u) \
    volatile struct uart *__u = (volatile struct uart *)UART_BASE;

static inline void uart_putc(volatile struct uart *u, uint32_t c) {
    while (!u->csr.tx_empty);
    u->tx_space = c;
}

static inline int uart_rx_ready(volatile struct uart *u) {
    return u->csr.rx_empty;
}

static inline char uart_getc(volatile struct uart *u) {
    while (!u->csr.rx_empty);
    return (uint32_t)u->rx_space;
}

static inline void
uart_gets(volatile struct uart *u, char *dst, size_t max_size) {
    while (uart_rx_ready(u) && max_size) {
        char c = uart_getc(u);
        if (c == '\n') break;
        *dst++ = c;
        max_size -= 1;
    }
    if (max_size != 0) *dst = 0;
}

static inline void uart_puts(volatile struct uart *u, const char *s) {
    while (*s) uart_putc(u, *s++);
}

static inline void uart_puti(volatile struct uart *u, int num) {
    if (num == 0) {
        uart_putc(u, '0');
        return;
    }
    char buf[15] = {};
    int i = 0;
    while (num) {
        buf[i++] = '0' + num % 10;
        num /= 10;
    }
    for (int j = 0; j < i / 2; ++j) {
        char tmp = buf[j];
        buf[j] = buf[i - j - 1];
        buf[i - j - 1] = tmp;
    }
    uart_puts(u, buf);
}

int main() {
    uart_declare(u);
    uart_puts(u, "Hello in test programm!\n");
    const char *passwd = "qwerty";
    char inp[17] = {};

    while (1) {
        uart_puts(u, "Enter passphrase: ");
        uart_gets(u, (char *)inp, 16);
        if (strcmp(inp, passwd) == 0) {
            uart_puts(u, "Correct passphrase: '");
            uart_puts(u, inp);
            uart_puts(u, "'!\n");
            break;
        } else {
            uart_puts(u, "Incorrect passphrase: '");
            uart_puts(u, inp);
            uart_puts(u, "'!\n\n");
        }
    }
    return 0;
}
