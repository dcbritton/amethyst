%struct.Matrix = type { i64** }
define dso_local void @new(i64 noundef %0, %struct.Matrix* noalias sret(%struct.Matrix) %1) {
  ; Handle parameters
  %3 = alloca i64
  store i64 %0, i64* %3
  %4 = getelementptr i64*, i64** null, i32 1
  %5 = ptrtoint i64** %4 to i64
  %6 = load i64, i64* %3
  %7 = mul i64 %5, %6
  %8 = call noalias i8* @malloc(i64 noundef %7)
  %9 = bitcast i8* %8 to i64**
  %10 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %1, i32 0, i32 0
  store i64** %9, i64*** %10

  ; Define i:int
  %11 = alloca i64
  %12 = add i64 0, 0
  store i64 %12, i64* %11
  ; End definition of i:int
  br label %cond0

cond0:
  ; Begin eq expr
  %13 = load i64, i64* %11
  %14 = load i64, i64* %3
  %15 = icmp ne i64 %13, %14
  ; End eq expr
  br i1 %15, label %body0, label %exit0

body0:
  %16 = getelementptr i64, i64* null, i32 1
  %17 = ptrtoint i64* %16 to i64
  %18 = load i64, i64* %3
  %19 = mul i64 %17, %18
  %20 = call noalias i8* @malloc(i64 noundef %19)
  %21 = bitcast i8* %20 to i64*
  %22 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %1, i32 0, i32 0
  %23 = load i64**, i64*** %22
  %24 = load i64, i64* %11
  %25 = getelementptr i64*, i64** %23, i64 %24
  store i64* %21, i64** %25
  ; Begin add expr
  %26 = load i64, i64* %11
  %27 = add i64 0, 1
  %28 = add i64 %26, %27
  ; End add expr
  store i64 %28, i64* %11
  br label %cond0

exit0:
  ret void
}

define dso_local i64 @main() {
  ; Handle parameters

  ; Define size:int
  %1 = alloca i64
  %2 = add i64 0, 4
  store i64 %2, i64* %1
  ; End definition of size:int

  ; Define a:Matrix
  %3 = alloca %struct.Matrix
  %4 = alloca %struct.Matrix ; Placeholder allocating space for struct return
  %5 = load i64, i64* %1
  call void @new(i64 noundef %5, %struct.Matrix* sret(%struct.Matrix) %4)
  %6 = getelementptr %struct.Matrix, %struct.Matrix* null, i32 1
  %7 = ptrtoint %struct.Matrix* %6 to i64
  %8 = bitcast %struct.Matrix* %3 to i8*
  %9 = bitcast %struct.Matrix* %4 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %8, i8* %9, i64 %7, i1 false)
  ; End definition of a:Matrix

  ; Define i:int
  %10 = alloca i64
  %11 = add i64 0, 0
  store i64 %11, i64* %10
  ; End definition of i:int
  br label %cond0

cond0:
  ; Begin eq expr
  %12 = load i64, i64* %10
  %13 = load i64, i64* %1
  %14 = icmp ne i64 %12, %13
  ; End eq expr
  br i1 %14, label %body0, label %exit0

body0:

  ; Define j:int
  %15 = alloca i64
  %16 = add i64 0, 0
  store i64 %16, i64* %15
  ; End definition of j:int
  br label %cond1

cond1:
  ; Begin eq expr
  %17 = load i64, i64* %15
  %18 = load i64, i64* %1
  %19 = icmp ne i64 %17, %18
  ; End eq expr
  br i1 %19, label %body1, label %exit1

body1:
  ; Begin add expr
  %20 = load i64, i64* %10
  %21 = load i64, i64* %15
  %22 = add i64 %20, %21
  ; End add expr
  %23 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %3, i32 0, i32 0 ; Getting ptr to member
  %24 = load i64**, i64*** %23
  %25 = load i64, i64* %10
  %26 = getelementptr i64*, i64** %24, i64 %25
  %27 = load i64*, i64** %26
  %28 = load i64, i64* %15
  %29 = getelementptr i64, i64* %27, i64 %28
  store i64 %22, i64* %29
  ; Begin add expr
  %30 = load i64, i64* %15
  %31 = add i64 0, 1
  %32 = add i64 %30, %31
  ; End add expr
  store i64 %32, i64* %15
  br label %cond1

exit1:
  ; Begin add expr
  %33 = load i64, i64* %10
  %34 = add i64 0, 1
  %35 = add i64 %33, %34
  ; End add expr
  store i64 %35, i64* %10
  br label %cond0

exit0:
  %36 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %3, i32 0, i32 0 ; Getting ptr to member
  %37 = load i64**, i64*** %36
  %38 = add i64 0, 1
  %39 = getelementptr i64*, i64** %37, i64 %38
  %40 = load i64*, i64** %39
  %41 = add i64 0, 3
  %42 = getelementptr i64, i64* %40, i64 %41
  %43 = load i64, i64* %42
  ret i64 %43
}

; Declarations of llvm intrinstics, may be unused
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg)
declare noalias i8* @malloc(i64 noundef)
