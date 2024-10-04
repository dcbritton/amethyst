%struct.U = type { i64, i64* }

%struct.T = type { i64*, i64, %struct.U*, %struct.T*, i64 }
define dso_local void @new(i64 noundef %0, %struct.T* noalias sret(%struct.T) %1) {
  ; Handle parameters
  %3 = alloca i64
  store i64 %0, i64* %3
  %4 = load i64, i64* %3
  %5 = getelementptr inbounds %struct.T, %struct.T* %1, i32 0, i32 1
  store i64 %4, i64* %5
  %6 = getelementptr inbounds %struct.T, %struct.T* %1, i32 0, i32 1
  %7 = load i64, i64* %6
  %8 = getelementptr inbounds %struct.T, %struct.T* %1, i32 0, i32 4
  store i64 %7, i64* %8
  %9 = alloca [4 x %struct.U]
  %10 = bitcast [4 x %struct.U]* %9 to %struct.U*
  %11 = getelementptr inbounds %struct.T, %struct.T* %1, i32 0, i32 2
  store %struct.U* %10, %struct.U** %11

  ; Define i:int
  %12 = alloca i64
  %13 = add i64 0, 0
  store i64 %13, i64* %12
  ; End definition of i:int
  br label %cond0

cond0:
  ; Begin eq expr
  %14 = load i64, i64* %12
  %15 = add i64 0, 4
  %16 = icmp ne i64 %14, %15
  ; End eq expr
  br i1 %16, label %body0, label %exit0

body0:
  %17 = load i64, i64* %12
  %18 = getelementptr inbounds %struct.T, %struct.T* %1, i32 0, i32 2
  %19 = load %struct.U*, %struct.U** %18
  %20 = load i64, i64* %12
  %21 = getelementptr %struct.U, %struct.U* %19, i64 %20
  %22 = getelementptr inbounds %struct.U, %struct.U* %21, i32 0, i32 0 ; Getting ptr to member
  store i64 %17, i64* %22
  ; Begin add expr
  %23 = load i64, i64* %12
  %24 = add i64 0, 1
  %25 = add i64 %23, %24
  ; End add expr
  store i64 %25, i64* %12
  br label %cond0

exit0:
  %26 = getelementptr inbounds %struct.T, %struct.T* %1, i32 0, i32 2
  %27 = load %struct.U*, %struct.U** %26
  %28 = add i64 0, 2
  %29 = getelementptr %struct.U, %struct.U* %27, i64 %28
  %30 = getelementptr inbounds %struct.U, %struct.U* %29, i32 0, i32 0 ; Getting ptr to member
  %31 = load i64, i64* %30
  %32 = getelementptr inbounds %struct.T, %struct.T* %1, i32 0, i32 4
  store i64 %31, i64* %32
  ret void
}

define dso_local i64 @main() {
  ; Handle parameters

  ; Define a:T
  %1 = alloca %struct.T
  %2 = alloca %struct.T ; Placeholder allocating space for struct return
  %3 = add i64 0, 13
  call void @new(i64 noundef %3, %struct.T* sret(%struct.T) %2)
  %4 = getelementptr %struct.T, %struct.T* null, i32 1
  %5 = ptrtoint %struct.T* %4 to i64
  %6 = bitcast %struct.T* %1 to i8*
  %7 = bitcast %struct.T* %2 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %6, i8* %7, i64 %5, i1 false)
  ; End definition of a:T
  ; Begin add expr
  %8 = getelementptr inbounds %struct.T, %struct.T* %1, i32 0, i32 4 ; Getting ptr to member
  %9 = load i64, i64* %8
  %10 = getelementptr inbounds %struct.T, %struct.T* %1, i32 0, i32 1 ; Getting ptr to member
  %11 = load i64, i64* %10
  %12 = add i64 %9, %11
  ; End add expr
  ret i64 %12
}

; Declarations of llvm intrinstics, may be unused
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg)
