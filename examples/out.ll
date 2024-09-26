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
define dso_local i32 @other(i32 noundef %0, i32 noundef %1) {
  ; Handle parameters
  %3 = alloca i32, align 4
  store i32 %0, i32* %3, align 4
  %4 = alloca i32, align 4
  store i32 %1, i32* %4, align 4

  ; Define c:int
  %5 = alloca i32, align 4
  ; Begin mult expr
  %6 = load i32, i32* %3, align 4
  %7 = add nsw i32 0, 1
  %8 = mul nsw i32 %6, %7
  ; End mult expr
  ; Begin mult expr
  %9 = load i32, i32* %3, align 4
  ; Begin mult expr
  %10 = load i32, i32* %4, align 4
  %11 = add nsw i32 0, 2
  %12 = mul nsw i32 %10, %11
  ; End mult expr
  %13 = mul nsw i32 %9, %12
  ; End mult expr
  %14 = call i32 @example(i32 noundef %8, i32 noundef %13)
  store i32 %14, i32* %5, align 4
  ; End definition of c:int
  %15 = load i32, i32* %5, align 4
  ret i32 %15
}
