```cpp
void async foo(){
  throw std::exception();
}
void async bar(){
  printf("aa\n");
  foo();
  printf("bb\n");
}

void main(){

  bar();
}
```
