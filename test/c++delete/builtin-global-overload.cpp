void operator delete(void* ptr)
{
}

int main()
{
  int* x = new int;
  delete(x);
  ::delete(x);
}
