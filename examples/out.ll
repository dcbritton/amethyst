define dso_local i32 @example(i32 noundef %0, i32 noundef %1) {
  ; Handle parameters
  %3 = alloca i32, align 4
  store i32 %0, i32* %3, align 4
  %4 = alloca i32, align 4
  store i32 %1, i32* %4, align 4

  ; Define c:int
  %5 = alloca i32, align 4
  ; Begin mult expr
  ; Begin mult expr
  %6 = load i32, i32* %3, align 4
  %7 = load i32, i32* %4, align 4
  %8 = mul nsw i32 %6, %7
  ; End mult expr
  %9 = add nsw i32 0, 112323232
  %10 = mul nsw i32 %8, %9
  ; End mult expr
  store i32 %10, i32* %5, align 4
  ; End definition of c:int
  %11 = load i32, i32* %5, align 4
  ret i32 %11
}
define dso_local i32 @other(i32 noundef %12, i32 noundef %13) {
  ; Handle parameters
  %15 = alloca i32, align 4
  store i32 %12, i32* %15, align 4
  %16 = alloca i32, align 4
  store i32 %13, i32* %16, align 4

  ; Define c:int
  %17 = alloca i32, align 4
  ; Begin mult expr
  %18 = load i32, i32* %15, align 4
  %19 = add nsw i32 0, 1
  %20 = mul nsw i32 %18, %19
  ; End mult expr
  ; Begin mult expr
  %21 = load i32, i32* %15, align 4
  ; Begin mult expr
  %22 = load i32, i32* %16, align 4
  %23 = add nsw i32 0, 2
  %24 = mul nsw i32 %22, %23
  ; End mult expr
  %25 = mul nsw i32 %21, %24
  ; End mult expr
  %26 = call i32 @example(i32 noundef %20, i32 noundef %25)
  store i32 %26, i32* %17, align 4
  ; End definition of c:int
  %27 = load i32, i32* %17, align 4
  ret i32 %27
}
