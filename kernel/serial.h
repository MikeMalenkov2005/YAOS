#ifndef SERIAL_H
#define SERIAL_H

#define UART_PARITY_NONE  0
#define UART_PARITY_ODD   1
#define UART_PARITY_EVEN  3
#define UART_PARITY_MARK  5
#define UART_PARITY_SPACE 7

extern int _init_com_port(int index, int baud_rate, int data_bits, int parity, int stop_bits);
extern int _read_com_port(int index);
extern int _write_com_port(int index, int data);

extern int _clear_first_ps2();
extern int _clear_second_ps2();
extern int _read_first_ps2();
extern int _read_second_ps2();
extern int _write_first_ps2(int byte);
extern int _write_second_ps2(int byte);

inline static int _wait_first_ps2() {
  int byte = _read_first_ps2();
  while (byte < 0) byte = _read_first_ps2();
  return byte;
}

inline static int _wait_second_ps2() {
  int byte = _read_second_ps2();
  while (byte < 0) byte = _read_second_ps2();
  return byte;
}

#endif

