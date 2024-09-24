define dso_local i32 @example(i32 noundef %0, i32 noundef %1) {
  ; Handle parameters
  %2 = alloca i32, align 4
  store i32 %0, i32* %2, align 4
  %3 = alloca i32, align 4
  store i32 %1, i32* %3, align 4

  ; Define c:int
  %4 = alloca i32, align 4
  ; Begin mult expr
  ; Begin mult expr
  %5 = load i32, i32* %2, align 4
  %6 = load i32, i32* %3, align 4
  %7 = mul nsw i32 %5, %6
  ; End mult expr
  %8 = load i32, i32* %2, align 4
  %9 = mul nsw i32 %7, %8
  ; End mult expr
  store i32 %9, i32* %4, align 4
  ; End definition of c:int
}
