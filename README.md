# lemom2

fiber-based(stackful) coroutine & async framework

yield
----
```cpp
coroutine<int> test(){
  yield(1);
  yield(2);
  yield(3);
}
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
