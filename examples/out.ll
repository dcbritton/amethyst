define dso_local i32** @ptrFunc(i32** noundef %0) {
  ; Handle parameters
  %2 = alloca i32**, align 4
  store i32** %0, i32*** %2, align 4
  %3 = load i32**, i32*** %2, align 4
  ret i32** %3
}

define dso_local i1 @example(i32 noundef %0, i32 noundef %1) {
  ; Handle parameters
  %3 = alloca i32, align 4
  store i32 %0, i32* %3, align 4
  %4 = alloca i32, align 4
  store i32 %1, i32* %4, align 4

  ; Define c:bool
  %5 = alloca i1, align 4
  ; Begin eq expr
  %6 = load i32, i32* %3, align 4
  %7 = load i32, i32* %4, align 4
  %8 = icmp eq i32 %6, %7
  ; End eq expr
  store i1 %8, i1* %5, align 4
  ; End definition of c:bool
  %9 = load i1, i1* %5, align 4
  ret i1 %9
}

define dso_local i32 @other(i32 noundef %0, i32 noundef %1) {
  ; Handle parameters
  %3 = alloca i32, align 4
  store i32 %0, i32* %3, align 4
  %4 = alloca i32, align 4
  store i32 %1, i32* %4, align 4
  ; Begin mult expr
  ; Begin mult expr
  %5 = load i32, i32* %3, align 4
  %6 = load i32, i32* %4, align 4
  %7 = mul i32 %5, %6
  ; End mult expr
  %8 = load i32, i32* %4, align 4
  %9 = sdiv i32 %7, %8
  ; End mult expr
  ret i32 %9
}

define dso_local i32 @looper() {
  ; Handle parameters
  br label %cond0

cond0:
  ; Begin eq expr
  %1 = add nsw i32 0, 1
  %2 = add nsw i32 0, 2
  %3 = icmp eq i32 %1, %2
  ; End eq expr
  br i1 %3, label %body0, label %exit0

body0:
  ; Begin mult expr
  %4 = add nsw i32 0, 1
  %5 = add nsw i32 0, 1
  %6 = mul i32 %4, %5
  ; End mult expr
  br label %cond1

cond1:
  ; Begin eq expr
  %7 = add nsw i32 0, 1
  %8 = add nsw i32 0, 2
  %9 = icmp eq i32 %7, %8
  ; End eq expr
  br i1 %9, label %body1, label %exit1

body1:
  ; Begin mult expr
  %10 = add nsw i32 0, 2
  %11 = add nsw i32 0, 2
  %12 = mul i32 %10, %11
  ; End mult expr
  br label %cond1

exit1:
  ; Begin mult expr
  %13 = add nsw i32 0, 3
  %14 = add nsw i32 0, 3
  %15 = mul i32 %13, %14
  ; End mult expr
  br label %cond0

exit0:
  ; Begin mult expr
  %16 = add nsw i32 0, 4
  %17 = add nsw i32 0, 4
  %18 = mul i32 %16, %17
  ; End mult expr
  %19 = add nsw i32 0, 1
  ret i32 %19
}

define dso_local i32 @main() {
  ; Handle parameters
  %1 = add nsw i32 0, 0
  ret i32 %1
}

