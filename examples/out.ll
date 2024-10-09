define dso_local i64 @main() {
  ; Primitive parameter allocations and stores
  ; End parameter handling

  ; Define i:int
  %1 = alloca i64
  %2 = add i64 0, 25
  store i64 %2, i64* %1
  ; End definition of i:int

  ; Define j:int
  %3 = alloca i64
  %4 = add i64 0, 2
  store i64 %4, i64* %3
  ; End definition of j:int
  ; Begin eq expr
  %5 = load i64, i64* %1
  %6 = add i64 0, 0
  %7 = icmp eq i64 %5, %6
  ; End eq expr
  br i1 %7, label %ifbody0, label %elsifcond0x0

ifbody0:
  ; Begin eq expr
  %8 = load i64, i64* %3
  %9 = add i64 0, 2
  %10 = icmp eq i64 %8, %9
  ; End eq expr
  br i1 %10, label %ifbody1, label %elsebody1

ifbody1:
  %11 = add i64 0, 199
  store i64 %11, i64* %3
  br label %exit1

elsebody1:
  %12 = add i64 0, 200
  store i64 %12, i64* %3
  br label %exit1

exit1:
  br label %exit0

elsifcond0x0:
  ; Begin eq expr
  %13 = load i64, i64* %1
  %14 = add i64 0, 1
  %15 = icmp eq i64 %13, %14
  ; End eq expr
  br i1 %15, label %elsifbody0x0, label %elsifcond0x1

elsifbody0x0:
  %16 = add i64 0, 201
  store i64 %16, i64* %3
  br label %exit0

elsifcond0x1:
  ; Begin eq expr
  %17 = load i64, i64* %1
  %18 = add i64 0, 2
  %19 = icmp eq i64 %17, %18
  ; End eq expr
  br i1 %19, label %elsifbody0x1, label %elsebody0

elsifbody0x1:
  %20 = add i64 0, 202
  store i64 %20, i64* %3
  br label %exit0

elsebody0:
  %21 = add i64 0, 203
  store i64 %21, i64* %3
  br label %exit0

exit0:
  %22 = load i64, i64* %3
  ret i64 %22
}

; Declarations of llvm intrinstics, may be unused
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg)
declare noalias i8* @malloc(i64 noundef)
declare void @free(i8* noundef)
