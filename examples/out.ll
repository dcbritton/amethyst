define dso_local i64 @ptrFunc(i64* noundef %0) {
  ; Handle parameters
  %2 = alloca i64*
  store i64* %0, i64** %2

  ; Define i:int
  %3 = alloca i64
  %4 = add i64 0, 1
  store i64 %4, i64* %3
  ; End definition of i:int
  br label %cond0

cond0:
  ; Begin eq expr
  %5 = load i64, i64* %3
  %6 = add i64 0, 33
  %7 = icmp ne i64 %5, %6
  ; End eq expr
  br i1 %7, label %body0, label %exit0

body0:
  %8 = add i64 0, 1
  %9 = load i64*, i64** %2
  %10 = load i64, i64* %3
  %11 = getelementptr i64, i64* %9, i64 %10
  store i64 %8, i64* %11
  ; Begin mult expr
  %12 = load i64, i64* %3
  %13 = add i64 0, 2
  %14 = mul i64 %12, %13
  ; End mult expr
  store i64 %14, i64* %3
  br label %cond0

exit0:
  %15 = add i64 0, 0
  ret i64 %15
}

define dso_local i64 @doubleSubscript(i64** noundef %0, i64 noundef %1) {
  ; Handle parameters
  %3 = alloca i64**
  store i64** %0, i64*** %3
  %4 = alloca i64
  store i64 %1, i64* %4
  %5 = add i64 0, 1
  %6 = load i64**, i64*** %3
  %7 = add i64 0, 0
  %8 = getelementptr i64*, i64** %6, i64 %7
  %9 = load i64*, i64** %8
  %10 = add i64 0, 0
  %11 = getelementptr i64, i64* %9, i64 %10
  store i64 %5, i64* %11
  %12 = load i64**, i64*** %3
  %13 = add i64 0, 0
  %14 = getelementptr i64*, i64** %12, i64 %13
  %15 = load i64*, i64** %14
  %16 = add i64 0, 0
  %17 = getelementptr i64, i64* %15, i64 %16
  %18 = load i64, i64* %17
  store i64 %18, i64* %4
  %19 = add i64 0, 1
  ret i64 %19
}

define dso_local i64 @main() {
  ; Handle parameters
  %1 = add i64 0, 0
  ret i64 %1
}

