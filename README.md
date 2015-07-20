# lemom2


yield
----
```cpp
coroutine<int> test(){
  yield(1);
  yield(2);
  yield(3);
}
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

time literals
----
```cpp
task<int> test(){
  await delay(1ms);
  await delay(1s);
}
```
