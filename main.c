#define VGA ((short*)0xB8000)

void print(char *string) {
  for (int i = 0; string[i]; ++i) {
    VGA[i] = string[i] | 0x0200;
  }
}

int kmain() {
  print("VeEeRyY CoOoOoL!!");
  return 0;
}

