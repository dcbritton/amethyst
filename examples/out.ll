define dso_local i64 @ptrFunc(i64* noundef %0) {
  ; Handle parameters
  %2 = alloca i64*
  store i64* %0, i64** %2

  ; Define i:int
  %3 = alloca i64
  %4 = add i64 0, 0
  store i64 %4, i64* %3
  ; End definition of i:int
  br label %cond0

cond0:
  ; Begin eq expr
  %5 = load i64, i64* %3
  %6 = add i64 0, 10
  %7 = icmp ne i64 %5, %6
  ; End eq expr
  br i1 %7, label %body0, label %exit0

body0:

  ; Define a:int*
  %8 = alloca i64*
  %9 = load i64*, i64** %2
  store i64* %9, i64** %8
  ; End definition of a:int*

  ; Define b:int
  %10 = alloca i64
  %11 = load i64*, i64** %2
  %12 = load i64, i64* %3
  %13 = getelementptr i64, i64* %11, i64 %12
  %14 = load i64, i64* %13
  store i64 %14, i64* %10
  ; End definition of b:int
  br label %cond0

exit0:
  %15 = add i64 0, 0
  ret i64 %15
}

define dso_local i64 @main() {
  ; Handle parameters
  %1 = add i64 0, 0
  ret i64 %1
}

