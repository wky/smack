class foo
{
public:
  int x;
  int y;
  ~foo() { x = 1;}
};

int main()
{
  class foo* x = new foo();
  delete(x);
}
