# graph.c

Better than cgraph.

Step 1. clone  
Step 2. ```$ make test```

##### Unit tests
- Write unit tests with ```unit_test```
- Write assertions with ```pass```
- Run tests with ```test```

Example from tests/test.c
```
unit_test(unit)
{
    pass(test, true, "%s must pass", "this test");
}

int main(int argc, char const *argv[])
{
    test(unit);
    return 0;
}
```

Output:
```
[ 100% ] unit (1 of 1)
```
