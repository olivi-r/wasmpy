(module
	(import "wasi_snapshot_preview1" "fd_write" (func $fd_write (param i32 i32 i32 i32) (result i32)))
	(global (export "global_i32") i32 (i32.const 666))
	(global (export "global_i64") i64 (i64.const 666))
	(global (export "global_f32") f32 (f32.const 666.6))
	(global (export "global_f64") f64 (f64.const 666.6))
	(table (export "table") 10 20 funcref)
	(memory (export "memory") 1 2)
	(func $print_int (param $num i64)
		(local $offset i32)
		(local $negative i32)
		(local.set $offset (i32.const 100))

		;; check if integer negative
		(if
			(i64.lt_s (local.get $num) (i64.const 0))
			(then
				(local.set $negative (i32.const 1))
				(local.set $num (i64.mul (local.get $num) (i64.const -1)))
			)
		)

		;; calculate and store digits: number rem 10
		(loop $digits
			(i64.store8 (local.get $offset) (i64.add (i64.const 48) (i64.rem_u (local.get $num) (i64.const 10))))
			(local.set $num (i64.div_u (local.get $num) (i64.const 10)))
			(local.set $offset (i32.sub (local.get $offset) (i32.const 1)))
			(br_if $digits (i64.ne (local.get $num) (i64.const 0)))
		)

		;; if negative add "-"
		(if
			(i32.ne (local.get $negative) (i32.const 0))
			(then
				(i32.store8 (local.get $offset) (i32.const 45))
				(local.set $offset (i32.sub (local.get $offset) (i32.const 1)))
			)
		)

		;; write number to stdout
		(i32.store (i32.const 0) (i32.add (local.get $offset) (i32.const 1)))
		(i32.store (i32.const 4) (i32.sub (i32.const 100) (local.get $offset)))
		(call $fd_write (i32.const 1) (i32.const 0) (i32.const 1) (i32.const 8))
		drop
	)
	(func $print_float (param $float f64)
		(local $num f64)

		;; print integer component
		(call $print_int (i64.trunc_f64_s (local.tee $num (f64.trunc (local.get $float)))))

		;; move fraction in front of point
		(local.set $float (f64.abs (f64.sub (local.get $float) (local.get $num))))
		(loop $frac
			(if
				(f64.gt (f64.sub (local.get $float) (f64.trunc (local.get $float))) (f64.const 0))
				(then
					(local.set $float (f64.mul (local.get $float) (f64.const 10)))
					(br $frac)
				)
			)
		)

		;; write "." to stdout
		(i32.store (i32.const 8) (i32.const 0x2E))
		(i64.store (i32.const 0) (i64.const 0x100000008))
		(call $fd_write (i32.const 1) (i32.const 0) (i32.const 1) (i32.const 0))
		drop

		;; print fractional component
		(call $print_int (i64.trunc_f64_u (local.get $float)))
	)
	(func (export "print"))
	(func $print_i32 (export "print_i32") (param $i i32)
		(call $print_int (i64.extend_i32_s (local.get $i)))

		;; write ": i32\n" to stdout
		(i64.store (i32.const 8) (i64.const 0xA323369203A))
		(i64.store (i32.const 0) (i64.const 0x600000008))
		(call $fd_write (i32.const 1) (i32.const 0) (i32.const 1) (i32.const 0))
		drop
	)
	(func $print_i64 (export "print_i64") (param $i i64)
		(call $print_int (local.get $i))

		;; write ": i64\n" to stdout
		(i64.store (i32.const 8) (i64.const 0xA343669203A))
		(i64.store (i32.const 0) (i64.const 0x600000008))
		(call $fd_write (i32.const 1) (i32.const 0) (i32.const 1) (i32.const 0))
		drop
	)
	(func $print_f32 (export "print_f32") (param $f f32)
		(call $print_float (f64.promote_f32 (local.get $f)))

		;; write ": f32\n" to stdout
		(i64.store (i32.const 8) (i64.const 0xA323366203A))
		(i64.store (i32.const 0) (i64.const 0x600000008))
		(call $fd_write (i32.const 1) (i32.const 0) (i32.const 1) (i32.const 0))
		drop
	)
	(func $print_f64 (export "print_f64") (param $f f64)
		(call $print_float (local.get $f))

		;; write ": f64\n" to stdout
		(i64.store (i32.const 8) (i64.const 0xA343666203A))
		(i64.store (i32.const 0) (i64.const 0x600000008))
		(call $fd_write (i32.const 1) (i32.const 0) (i32.const 1) (i32.const 0))
		drop
	)
	(func $print_i32_f32 (export "print_i32_f32") (param $i i32) (param $f f32)
		(call $print_i32 (local.get $i))
		(call $print_f32 (local.get $f))
	)
	(func $print_f64_f64 (export "print_f64_f64") (param $f0 f64) (param $f1 f64)
		(call $print_f64 (local.get $f0))
		(call $print_f64 (local.get $f1))
	)
	(func (export "_start")
		(call $print_f64_f64 (f64.const 69) (f64.const -28.7))
	)
)
