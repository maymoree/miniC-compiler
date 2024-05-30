target triple = "x86_64-pc-linux-gnu"

declare void @print(i32)

declare i32 @read()

define i32 @func(i32 %0) {
  %a = alloca i32, align 4
  %i = alloca i32, align 4
  %max = alloca i32, align 4
  %n = alloca i32, align 4
  %ret_ref = alloca i32, align 4
  store i32 %0, ptr %n, align 4
  store i32 0, ptr %i, align 4
  store i32 0, ptr %max, align 4
  br label %4

2:                                                ; preds = %13
  %3 = load i32, ptr %ret_ref, align 4
  ret i32 %3

4:                                                ; preds = %17, %1
  %5 = load i32, ptr %i, align 4
  %6 = load i32, ptr %n, align 4
  %7 = icmp slt i32 %5, %6
  br i1 %7, label %8, label %13

8:                                                ; preds = %4
  %9 = call i32 @read()
  store i32 %9, ptr %a, align 4
  %10 = load i32, ptr %a, align 4
  %11 = load i32, ptr %max, align 4
  %12 = icmp sgt i32 %10, %11
  br i1 %12, label %15, label %17

13:                                               ; preds = %4
  %14 = load i32, ptr %max, align 4
  store i32 %14, ptr %ret_ref, align 4
  br label %2

15:                                               ; preds = %8
  %16 = load i32, ptr %a, align 4
  store i32 %16, ptr %max, align 4
  br label %17

17:                                               ; preds = %15, %8
  %18 = load i32, ptr %i, align 4
  %19 = add i32 %18, 1
  store i32 %19, ptr %i, align 4
  br label %4
}
