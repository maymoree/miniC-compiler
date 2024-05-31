target triple = "x86_64-pc-linux-gnu"

declare void @print(i32)

declare i32 @read()

define i32 @func(i32 %0) {
  %a1 = alloca i32, align 4
  %a2 = alloca i32, align 4
  %i = alloca i32, align 4
  %n = alloca i32, align 4
  %t = alloca i32, align 4
  %ret_ref = alloca i32, align 4
  store i32 %0, ptr %n, align 4
  %2 = call i32 @read()
  store i32 %2, ptr %a1, align 4
  %3 = call i32 @read()
  store i32 %3, ptr %a2, align 4
  store i32 0, ptr %i, align 4
  br label %6

4:                                                ; preds = %21
  %5 = load i32, ptr %ret_ref, align 4
  ret i32 %5

6:                                                ; preds = %10, %1
  %7 = load i32, ptr %i, align 4
  %8 = load i32, ptr %n, align 4
  %9 = icmp slt i32 %7, %8
  br i1 %9, label %10, label %21

10:                                               ; preds = %6
  %11 = load i32, ptr %i, align 4
  call void @print(i32 %11)
  %12 = load i32, ptr %a1, align 4
  call void @print(i32 %12)
  %13 = load i32, ptr %a1, align 4
  %14 = load i32, ptr %a2, align 4
  %15 = add i32 %13, %14
  store i32 %15, ptr %t, align 4
  %16 = load i32, ptr %a1, align 4
  call void @print(i32 %16)
  %17 = load i32, ptr %a2, align 4
  store i32 %17, ptr %a1, align 4
  %18 = load i32, ptr %t, align 4
  store i32 %18, ptr %a2, align 4
  %19 = load i32, ptr %i, align 4
  %20 = add i32 %19, 1
  store i32 %20, ptr %i, align 4
  br label %6

21:                                               ; preds = %6
  %22 = load i32, ptr %a1, align 4
  %23 = load i32, ptr %a2, align 4
  %24 = add i32 %22, %23
  store i32 %24, ptr %ret_ref, align 4
  br label %4
}
