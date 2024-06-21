#ifndef SERIAL_H
#define SERIAL_H

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

