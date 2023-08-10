(module
    (global f32 f32.const 0)
    (global (mut f64) f64.const 0)
    (global (mut i32) i32.const 0)
    (global (mut i64) i64.const 0)

    ;; local.get
    (func (param f32) (result f32) local.get 0)
    (func (param f64) (result f64) local.get 0)
    (func (param i32) (result i32) local.get 0)
    (func (param i64) (result i64) local.get 0)

    ;; local.set
    (func (param f32) (result f32) (local f32) local.get 0 local.set 1 local.get 1)
    (func (param f64) (result f64) (local f64) local.get 0 local.set 1 local.get 1)
    (func (param i32) (result i32) (local i32) local.get 0 local.set 1 local.get 1)
    (func (param i64) (result i64) (local i64) local.get 0 local.set 1 local.get 1)

    ;; local.tee
    (func (param f32) (result i32) (local f32) local.get 0 local.tee 1)
    (func (param f64) (result i32) (local f64) local.get 0 local.tee 1)
    (func (param i32) (result i32) (local i32) local.get 0 local.tee 1)
    (func (param i64) (result i32) (local i64) local.get 0 local.tee 1)

    ;; global.set
    (func (param f32) local.get 0 global.set 0)
    (func (param f64) local.get 0 global.set 1)
    (func (param i32) local.get 0 global.set 2)
    (func (param i64) local.get 0 global.set 3)

    ;; global.get
    (func (result f32) global.get 0)
    (func (result f64) global.get 1)
    (func (result i32) global.get 2)
    (func (result i64) global.get 3)
)
