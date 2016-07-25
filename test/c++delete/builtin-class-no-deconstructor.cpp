class foo
{
  int x;
  int y;
};

int main()
{
  class foo* x = new foo();
  delete(x);
}
