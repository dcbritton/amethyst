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
  %5 = load i64, i64* %3
  ret i64 %5
  %6 = add i64 0, 123
  store i64 %6, i64* %1
}

; Declarations of llvm intrinstics, may be unused
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg)
declare noalias i8* @malloc(i64 noundef)
declare void @free(i8* noundef)
