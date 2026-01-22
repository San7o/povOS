int sum(int a, int b)
{
  return a + b;
}

int kernel_main(void)
{

  int a = 10;
  int b = 20;
  int c = sum(a, b);
  return (c != 30);
}
