target triple = "x86_64-pc-linux-gnu"

declare void @print(i32)

declare i32 @read()

define i32 @func(i32 %0) {
  %a = alloca i32, align 4
  %a1 = alloca i32, align 4
  %b = alloca i32, align 4
  %p = alloca i32, align 4
  %ret_ref = alloca i32, align 4
  store i32 %0, ptr %p, align 4
  store i32 0, ptr %a, align 4
  %2 = load i32, ptr %p, align 4
  %3 = icmp slt i32 %2, 0
  br i1 %3, label %6, label %7

4:                                                ; preds = %16
  %5 = load i32, ptr %ret_ref, align 4
  ret i32 %5

6:                                                ; preds = %1
  store i32 10, ptr %a, align 4
  br label %16

7:                                                ; preds = %1
  store i32 2, ptr %a, align 4
  store i32 0, ptr %b, align 4
  br label %8

8:                                                ; preds = %12, %7
  %9 = load i32, ptr %b, align 4
  %10 = load i32, ptr %p, align 4
  %11 = icmp slt i32 %9, %10
  br i1 %11, label %12, label %15

12:                                               ; preds = %8
  %13 = load i32, ptr %b, align 4
  %14 = add i32 %13, 2
  store i32 %14, ptr %b, align 4
  br label %8

15:                                               ; preds = %8
  br label %16

16:                                               ; preds = %15, %6
  %17 = load i32, ptr %a, align 4
  %18 = load i32, ptr %b, align 4
  %19 = add i32 %17, %18
  store i32 %19, ptr %ret_ref, align 4
  br label %4
}
