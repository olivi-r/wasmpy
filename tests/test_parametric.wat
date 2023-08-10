(module
    (func (param f32) local.get 0 drop)
    (func (param f64) local.get 0 drop)
    (func (param i32) local.get 0 drop)
    (func (param i64) local.get 0 drop)

    (func (param f32 f32 i32) (result f32) local.get 0 local.get 1 local.get 2 select)
    (func (param f64 f64 i32) (result f64) local.get 0 local.get 1 local.get 2 select)
    (func (param i32 i32 i32) (result i32) local.get 0 local.get 1 local.get 2 select)
    (func (param i64 i64 i32) (result i64) local.get 0 local.get 1 local.get 2 select)
)
