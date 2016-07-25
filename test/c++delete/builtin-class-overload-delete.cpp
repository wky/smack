class foo
{
public:
  int x;
  int y;
  ~foo() {x = 1;}
  static void operator delete(void *ptr) {}
};

int main()
{
  class foo* x = new foo();
  delete(x);
}
