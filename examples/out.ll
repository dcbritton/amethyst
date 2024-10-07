@.global.a = dso_local global i64 zeroinitializer
@.global.b = dso_local global float zeroinitializer
%struct.Matrix = type { i64**, i64 }
define dso_local void @Matrix.new$int(i64 noundef %0, %struct.Matrix* noalias sret(%struct.Matrix) %1) {
  ; Primitive parameter allocations and stores
  %3 = alloca i64
  store i64 %0, i64* %3
  ; End parameter handling
  %4 = load i64, i64* %3
  %5 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %1, i32 0, i32 1
  store i64 %4, i64* %5
  %6 = getelementptr i64*, i64** null, i32 1
  %7 = ptrtoint i64** %6 to i64
  %8 = load i64, i64* %3
  %9 = mul i64 %7, %8
  %10 = call noalias i8* @malloc(i64 noundef %9)
  %11 = bitcast i8* %10 to i64**
  %12 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %1, i32 0, i32 0
  store i64** %11, i64*** %12

  ; Define i:int
  %13 = alloca i64
  %14 = add i64 0, 0
  store i64 %14, i64* %13
  ; End definition of i:int
  br label %cond0

cond0:
  ; Begin eq expr
  %15 = load i64, i64* %13
  %16 = load i64, i64* %3
  %17 = icmp ne i64 %15, %16
  ; End eq expr
  br i1 %17, label %body0, label %exit0

body0:
  %18 = getelementptr i64, i64* null, i32 1
  %19 = ptrtoint i64* %18 to i64
  %20 = load i64, i64* %3
  %21 = mul i64 %19, %20
  %22 = call noalias i8* @malloc(i64 noundef %21)
  %23 = bitcast i8* %22 to i64*
  %24 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %1, i32 0, i32 0
  %25 = load i64**, i64*** %24
  %26 = load i64, i64* %13
  %27 = getelementptr i64*, i64** %25, i64 %26
  store i64* %23, i64** %27
  ; Begin add expr
  %28 = load i64, i64* %13
  %29 = add i64 0, 1
  %30 = add i64 %28, %29
  ; End add expr
  store i64 %30, i64* %13
  br label %cond0

exit0:
  ret void
}
define dso_local void @Matrix.new$Matrix(%struct.Matrix* noundef byval(%struct.Matrix) %0, %struct.Matrix* noalias sret(%struct.Matrix) %1) {
  ; Primitive parameter allocations and stores
  ; End parameter handling
  %3 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %0, i32 0, i32 1 ; Getting ptr to member
  %4 = load i64, i64* %3
  %5 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %1, i32 0, i32 1
  store i64 %4, i64* %5
  %6 = getelementptr i64*, i64** null, i32 1
  %7 = ptrtoint i64** %6 to i64
  %8 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %1, i32 0, i32 1
  %9 = load i64, i64* %8
  %10 = mul i64 %7, %9
  %11 = call noalias i8* @malloc(i64 noundef %10)
  %12 = bitcast i8* %11 to i64**
  %13 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %1, i32 0, i32 0
  store i64** %12, i64*** %13

  ; Define i:int
  %14 = alloca i64
  %15 = add i64 0, 0
  store i64 %15, i64* %14
  ; End definition of i:int
  br label %cond0

cond0:
  ; Begin eq expr
  %16 = load i64, i64* %14
  %17 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %1, i32 0, i32 1
  %18 = load i64, i64* %17
  %19 = icmp ne i64 %16, %18
  ; End eq expr
  br i1 %19, label %body0, label %exit0

body0:
  %20 = getelementptr i64, i64* null, i32 1
  %21 = ptrtoint i64* %20 to i64
  %22 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %1, i32 0, i32 1
  %23 = load i64, i64* %22
  %24 = mul i64 %21, %23
  %25 = call noalias i8* @malloc(i64 noundef %24)
  %26 = bitcast i8* %25 to i64*
  %27 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %1, i32 0, i32 0
  %28 = load i64**, i64*** %27
  %29 = load i64, i64* %14
  %30 = getelementptr i64*, i64** %28, i64 %29
  store i64* %26, i64** %30
  ; Begin add expr
  %31 = load i64, i64* %14
  %32 = add i64 0, 1
  %33 = add i64 %31, %32
  ; End add expr
  store i64 %33, i64* %14
  br label %cond0

exit0:
  %34 = add i64 0, 0
  store i64 %34, i64* %14
  br label %cond1

cond1:
  ; Begin eq expr
  %35 = load i64, i64* %14
  %36 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %1, i32 0, i32 1
  %37 = load i64, i64* %36
  %38 = icmp ne i64 %35, %37
  ; End eq expr
  br i1 %38, label %body1, label %exit1

body1:

  ; Define j:int
  %39 = alloca i64
  %40 = add i64 0, 0
  store i64 %40, i64* %39
  ; End definition of j:int
  br label %cond2

cond2:
  ; Begin eq expr
  %41 = load i64, i64* %39
  %42 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %1, i32 0, i32 1
  %43 = load i64, i64* %42
  %44 = icmp ne i64 %41, %43
  ; End eq expr
  br i1 %44, label %body2, label %exit2

body2:
  %45 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %0, i32 0, i32 0 ; Getting ptr to member
  %46 = load i64**, i64*** %45
  %47 = load i64, i64* %14
  %48 = getelementptr i64*, i64** %46, i64 %47
  %49 = load i64*, i64** %48
  %50 = load i64, i64* %39
  %51 = getelementptr i64, i64* %49, i64 %50
  %52 = load i64, i64* %51
  %53 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %1, i32 0, i32 0
  %54 = load i64**, i64*** %53
  %55 = load i64, i64* %14
  %56 = getelementptr i64*, i64** %54, i64 %55
  %57 = load i64*, i64** %56
  %58 = load i64, i64* %39
  %59 = getelementptr i64, i64* %57, i64 %58
  store i64 %52, i64* %59
  ; Begin add expr
  %60 = load i64, i64* %39
  %61 = add i64 0, 1
  %62 = add i64 %60, %61
  ; End add expr
  store i64 %62, i64* %39
  br label %cond2

exit2:
  ; Begin add expr
  %63 = load i64, i64* %14
  %64 = add i64 0, 1
  %65 = add i64 %63, %64
  ; End add expr
  store i64 %65, i64* %14
  br label %cond1

exit1:
  ret void
}
define dso_local void @Matrix.new$Matrix.(%struct.Matrix* noundef %0, %struct.Matrix* noalias sret(%struct.Matrix) %1) {
  ; Primitive parameter allocations and stores
  %3 = alloca %struct.Matrix*
  store %struct.Matrix* %0, %struct.Matrix** %3
  ; End parameter handling
  %4 = load %struct.Matrix*, %struct.Matrix** %3
  %5 = add i64 0, 0
  %6 = getelementptr %struct.Matrix, %struct.Matrix* %4, i64 %5
  %7 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %6, i32 0, i32 1 ; Getting ptr to member
  %8 = load i64, i64* %7
  %9 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %1, i32 0, i32 1
  store i64 %8, i64* %9
  %10 = getelementptr i64*, i64** null, i32 1
  %11 = ptrtoint i64** %10 to i64
  %12 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %1, i32 0, i32 1
  %13 = load i64, i64* %12
  %14 = mul i64 %11, %13
  %15 = call noalias i8* @malloc(i64 noundef %14)
  %16 = bitcast i8* %15 to i64**
  %17 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %1, i32 0, i32 0
  store i64** %16, i64*** %17

  ; Define i:int
  %18 = alloca i64
  %19 = add i64 0, 0
  store i64 %19, i64* %18
  ; End definition of i:int
  br label %cond0

cond0:
  ; Begin eq expr
  %20 = load i64, i64* %18
  %21 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %1, i32 0, i32 1
  %22 = load i64, i64* %21
  %23 = icmp ne i64 %20, %22
  ; End eq expr
  br i1 %23, label %body0, label %exit0

body0:
  %24 = getelementptr i64, i64* null, i32 1
  %25 = ptrtoint i64* %24 to i64
  %26 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %1, i32 0, i32 1
  %27 = load i64, i64* %26
  %28 = mul i64 %25, %27
  %29 = call noalias i8* @malloc(i64 noundef %28)
  %30 = bitcast i8* %29 to i64*
  %31 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %1, i32 0, i32 0
  %32 = load i64**, i64*** %31
  %33 = load i64, i64* %18
  %34 = getelementptr i64*, i64** %32, i64 %33
  store i64* %30, i64** %34
  ; Begin add expr
  %35 = load i64, i64* %18
  %36 = add i64 0, 1
  %37 = add i64 %35, %36
  ; End add expr
  store i64 %37, i64* %18
  br label %cond0

exit0:
  %38 = add i64 0, 0
  store i64 %38, i64* %18
  br label %cond1

cond1:
  ; Begin eq expr
  %39 = load i64, i64* %18
  %40 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %1, i32 0, i32 1
  %41 = load i64, i64* %40
  %42 = icmp ne i64 %39, %41
  ; End eq expr
  br i1 %42, label %body1, label %exit1

body1:

  ; Define j:int
  %43 = alloca i64
  %44 = add i64 0, 0
  store i64 %44, i64* %43
  ; End definition of j:int
  br label %cond2

cond2:
  ; Begin eq expr
  %45 = load i64, i64* %43
  %46 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %1, i32 0, i32 1
  %47 = load i64, i64* %46
  %48 = icmp ne i64 %45, %47
  ; End eq expr
  br i1 %48, label %body2, label %exit2

body2:
  %49 = load %struct.Matrix*, %struct.Matrix** %3
  %50 = add i64 0, 0
  %51 = getelementptr %struct.Matrix, %struct.Matrix* %49, i64 %50
  %52 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %51, i32 0, i32 0 ; Getting ptr to member
  %53 = load i64**, i64*** %52
  %54 = load i64, i64* %18
  %55 = getelementptr i64*, i64** %53, i64 %54
  %56 = load i64*, i64** %55
  %57 = load i64, i64* %43
  %58 = getelementptr i64, i64* %56, i64 %57
  %59 = load i64, i64* %58
  %60 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %1, i32 0, i32 0
  %61 = load i64**, i64*** %60
  %62 = load i64, i64* %18
  %63 = getelementptr i64*, i64** %61, i64 %62
  %64 = load i64*, i64** %63
  %65 = load i64, i64* %43
  %66 = getelementptr i64, i64* %64, i64 %65
  store i64 %59, i64* %66
  ; Begin add expr
  %67 = load i64, i64* %43
  %68 = add i64 0, 1
  %69 = add i64 %67, %68
  ; End add expr
  store i64 %69, i64* %43
  br label %cond2

exit2:
  ; Begin add expr
  %70 = load i64, i64* %18
  %71 = add i64 0, 1
  %72 = add i64 %70, %71
  ; End add expr
  store i64 %72, i64* %18
  br label %cond1

exit1:
  ret void
}

@.global.mat = dso_local global %struct.Matrix zeroinitializer
define dso_local i64 @foo$int$Matrix(i64 noundef %0, %struct.Matrix* noundef byval(%struct.Matrix) %1) {
  ; Primitive parameter allocations and stores
  %3 = alloca i64
  store i64 %0, i64* %3
  ; End parameter handling
  ; Begin add expr
  %4 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %1, i32 0, i32 1 ; Getting ptr to member
  %5 = load i64, i64* %4
  %6 = load i64, i64* %3
  %7 = add i64 %5, %6
  ; End add expr
  ret i64 %7
}

define dso_local i64 @main() {
  ; Primitive parameter allocations and stores
  ; End parameter handling

  ; Define size:int
  %1 = alloca i64
  %2 = add i64 0, 12
  store i64 %2, i64* %1
  ; End definition of size:int

  ; Define mat:Matrix
  %3 = alloca %struct.Matrix
  %4 = alloca %struct.Matrix ; Placeholder allocating space for struct return
  %5 = load i64, i64* %1
  call void @Matrix.new$int(i64 noundef %5, %struct.Matrix* sret(%struct.Matrix) %4)
  %6 = getelementptr %struct.Matrix, %struct.Matrix* null, i32 1
  %7 = ptrtoint %struct.Matrix* %6 to i64
  %8 = bitcast %struct.Matrix* %3 to i8*
  %9 = bitcast %struct.Matrix* %4 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %8, i8* %9, i64 %7, i1 false)
  ; End definition of mat:Matrix

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
  ; Begin add expr
  %20 = load i64, i64* %10
  %21 = load i64, i64* %15
  %22 = add i64 %20, %21
  ; End add expr
  %23 = add i64 0, -1
  %24 = add i64 %22, %23
  ; End add expr
  %25 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %3, i32 0, i32 0 ; Getting ptr to member
  %26 = load i64**, i64*** %25
  %27 = load i64, i64* %10
  %28 = getelementptr i64*, i64** %26, i64 %27
  %29 = load i64*, i64** %28
  %30 = load i64, i64* %15
  %31 = getelementptr i64, i64* %29, i64 %30
  store i64 %24, i64* %31
  ; Begin add expr
  %32 = load i64, i64* %15
  %33 = add i64 0, 1
  %34 = add i64 %32, %33
  ; End add expr
  store i64 %34, i64* %15
  br label %cond1

exit1:
  ; Begin add expr
  %35 = load i64, i64* %10
  %36 = add i64 0, 1
  %37 = add i64 %35, %36
  ; End add expr
  store i64 %37, i64* %10
  br label %cond0

exit0:
  %38 = alloca %struct.Matrix ; Placeholder allocating space for struct return
  call void @Matrix.new$Matrix(%struct.Matrix* noundef byval(%struct.Matrix) %3, %struct.Matrix* sret(%struct.Matrix) %38)
  %39 = bitcast %struct.Matrix* @.global.mat to %struct.Matrix* ; Workaround to use globals in current register management system
  %40 = getelementptr %struct.Matrix, %struct.Matrix* null, i32 1
  %41 = ptrtoint %struct.Matrix* %40 to i64
  %42 = bitcast %struct.Matrix* %39 to i8*
  %43 = bitcast %struct.Matrix* %38 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %42, i8* %43, i64 %41, i1 false)
  %44 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %3, i32 0, i32 1 ; Getting ptr to member
  %45 = load i64, i64* %44
  %46 = bitcast i64* @.global.a to i64* ; Workaround to use globals in current register management system
  store i64 %45, i64* %46

  ; Define placeholder:int
  %47 = alloca i64
  ; Begin add expr
  ; Begin add expr
  %48 = bitcast i64* @.global.a to i64* ; Workaround to use globals in current register management system
  %49 = load i64, i64* %48
  %50 = bitcast %struct.Matrix* @.global.mat to %struct.Matrix* ; Workaround to use globals in current register management system
  %51 = call i64 @foo$int$Matrix(i64 noundef %49, %struct.Matrix* noundef byval(%struct.Matrix) %50)
  %52 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %3, i32 0, i32 0 ; Getting ptr to member
  %53 = load i64**, i64*** %52
  ; Begin add expr
  %54 = load i64, i64* %1
  %55 = add i64 0, 1
  %56 = sub i64 %54, %55
  ; End add expr
  %57 = getelementptr i64*, i64** %53, i64 %56
  %58 = load i64*, i64** %57
  ; Begin add expr
  %59 = load i64, i64* %1
  %60 = add i64 0, 1
  %61 = sub i64 %59, %60
  ; End add expr
  %62 = getelementptr i64, i64* %58, i64 %61
  %63 = load i64, i64* %62
  %64 = add i64 %51, %63
  ; End add expr
  %65 = add i64 0, -123
  %66 = sub i64 %64, %65
  ; End add expr
  store i64 %66, i64* %47
  ; End definition of placeholder:int

  ; Define k:int
  %67 = alloca i64
  %68 = add i64 0, 0
  store i64 %68, i64* %67
  ; End definition of k:int

  ; Define boolTest:bool
  %69 = alloca i1
  %70 = add i1 0, 1
  store i1 %70, i1* %69
  ; End definition of boolTest:bool
  br label %cond2

cond2:
  %71 = load i1, i1* %69
  br i1 %71, label %body2, label %exit2

body2:
  ; Begin add expr
  %72 = load i64, i64* %67
  %73 = add i64 0, 10
  %74 = add i64 %72, %73
  ; End add expr
  store i64 %74, i64* %67
  br label %cond3

cond3:
  %75 = load i1, i1* %69
  br i1 %75, label %body3, label %exit3

body3:
  %76 = add i1 0, 0
  store i1 %76, i1* %69
  br label %cond3

exit3:
  br label %cond2

exit2:
  %77 = load i64, i64* %67
  ret i64 %77
}

; Declarations of llvm intrinstics, may be unused
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg)
declare noalias i8* @malloc(i64 noundef)
