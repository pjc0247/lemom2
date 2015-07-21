# lemom2

fiber-based(stackful) coroutine & async framework

yield
----
```cpp
coroutine<int> test(){
  return coroutine<int>::create([](){
    yield(1);
    yield(2);
    yield(3);
  });
}

auto it = test();

std::cout<< it.next(); // 1
std::cout<< it.next(); // 2
std::cout<< it.next(); // 3
```
```cpp
for(int v : test())
  std::cout<< v; // 123
```

```rb
def test
  yield 1
  yield 2
  yield 3
end
```

await
----
```cpp
task<int> test_async(){
  return task::create([](){
    int calc = 0;
    for(int i=0;i<SOMEBIG;i++){
      calc += calcaclaclaclacl;
    }
    return calc;
  });
}

auto result = await test_async();

std::cout<< result;
```
```rb
def test_async
  Task::create do
    # ....
    calc_result
  end
end
```
```rb
# with roap
#-- async
def test_async
  # ....
  return calc_result
end
```

time literals
----
```cpp
task<int> test(){
  await delay(1ms);
  await delay(1s);
}
```
```rb
def test
  await delay ms 1
  await delay s 2
end
```
