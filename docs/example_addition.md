# Addition

<!-- tabs:start -->

#### **Python**

```python
import wasmpy
import addition

print(addition.add(5, 7))
```

#### **WebAssembly**

```wasm
;; addition.wat

(module
  (func (export "add") (param i32 i32) (result i32)
    (i32.add (local.get 0) (local.get 1))
  )
)
```

#### **Output**

```
12
```

<!-- tabs:end -->
