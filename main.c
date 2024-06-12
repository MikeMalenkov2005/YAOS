#define VGA ((short*)0xB8000)

void print(char *string) {
  for (int i = 0; string[i]; ++i) {
    VGA[i] = string[i] | 0x0200;
  }
}

int kcall(int fnid, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5) {
  return 0;
}

int kmain() {
  print("VeEeRyY CoOoOoL!!");
  return 0;
}

