define dso_local i64 @main() {
  ; Handle parameters

  ; Define arr:int**
  %1 = alloca i64**
  %2 = alloca [3 x i64*]
  %3 = bitcast [3 x i64*]* %2 to i64**
  store i64** %3, i64*** %1
  ; End definition of arr:int**

  ; Define i:int
  %4 = alloca i64
  %5 = add i64 0, 0
  store i64 %5, i64* %4
  ; End definition of i:int
  br label %cond0

cond0:
  ; Begin eq expr
  %6 = load i64, i64* %4
  %7 = add i64 0, 3
  %8 = icmp ne i64 %6, %7
  ; End eq expr
  br i1 %8, label %body0, label %exit0

body0:
  %9 = alloca [3 x i64]
  %10 = bitcast [3 x i64]* %9 to i64*
  %11 = load i64**, i64*** %1
  %12 = load i64, i64* %4
  %13 = getelementptr i64*, i64** %11, i64 %12
  store i64* %10, i64** %13
  ; Begin add expr
  %14 = load i64, i64* %4
  %15 = add i64 0, 1
  %16 = add i64 %14, %15
  ; End add expr
  store i64 %16, i64* %4
  br label %cond0

exit0:

  ; Define k:int
  %17 = alloca i64
  %18 = add i64 0, 0
  store i64 %18, i64* %17
  ; End definition of k:int
  br label %cond1

cond1:
  ; Begin eq expr
  %19 = load i64, i64* %17
  %20 = add i64 0, 3
  %21 = icmp ne i64 %19, %20
  ; End eq expr
  br i1 %21, label %body1, label %exit1

body1:

  ; Define j:int
  %22 = alloca i64
  %23 = add i64 0, 0
  store i64 %23, i64* %22
  ; End definition of j:int
  br label %cond2

cond2:
  ; Begin eq expr
  %24 = load i64, i64* %22
  %25 = add i64 0, 3
  %26 = icmp ne i64 %24, %25
  ; End eq expr
  br i1 %26, label %body2, label %exit2

body2:
  ; Begin add expr
  %27 = load i64, i64* %17
  %28 = load i64, i64* %22
  %29 = add i64 %27, %28
  ; End add expr
  %30 = load i64**, i64*** %1
  %31 = load i64, i64* %17
  %32 = getelementptr i64*, i64** %30, i64 %31
  %33 = load i64*, i64** %32
  %34 = load i64, i64* %22
  %35 = getelementptr i64, i64* %33, i64 %34
  store i64 %29, i64* %35
  ; Begin add expr
  %36 = load i64, i64* %22
  %37 = add i64 0, 1
  %38 = add i64 %36, %37
  ; End add expr
  store i64 %38, i64* %22
  br label %cond2

exit2:
  ; Begin add expr
  %39 = load i64, i64* %17
  %40 = add i64 0, 1
  %41 = add i64 %39, %40
  ; End add expr
  store i64 %41, i64* %17
  br label %cond1

exit1:
  %42 = load i64**, i64*** %1
  %43 = add i64 0, 2
  %44 = getelementptr i64*, i64** %42, i64 %43
  %45 = load i64*, i64** %44
  %46 = add i64 0, 0
  %47 = getelementptr i64, i64* %45, i64 %46
  %48 = load i64, i64* %47
  ret i64 %48
}

