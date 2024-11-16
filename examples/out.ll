define dso_local i64 @main() {
  ; Primitive parameter allocations and stores
  ; End parameter handling

  ; Define str:char*
  %r1 = alloca i8*
  %r2 = alloca [4 x i8]
  %r3 = bitcast [4 x i8]* %r2 to i8*
  store i8* %r3, i8** %r1
  ; End definition of str:char*
  %r4 = add i8 0, 87 ; Char literal
  %r5 = load i8*, i8** %r1
  %r6 = add i64 0, 0
  %r7 = getelementptr i8, i8* %r5, i64 %r6
  store i8 %r4, i8* %r7
  %r8 = add i8 0, 79 ; Char literal
  %r9 = load i8*, i8** %r1
  %r10 = add i64 0, 1
  %r11 = getelementptr i8, i8* %r9, i64 %r10
  store i8 %r8, i8* %r11
  %r12 = add i8 0, 87 ; Char literal
  %r13 = load i8*, i8** %r1
  %r14 = add i64 0, 2
  %r15 = getelementptr i8, i8* %r13, i64 %r14
  store i8 %r12, i8* %r15
  %r16 = add i8 0, 0 ; Char literal
  %r17 = load i8*, i8** %r1
  %r18 = add i64 0, 3
  %r19 = getelementptr i8, i8* %r17, i64 %r18
  store i8 %r16, i8* %r19
  %r20 = load i8*, i8** %r1
  %r21 = call i32 (i8*, ...) bitcast (i32 (...)* @puts to i32 (i8*, ...)*)(i8* noundef %r20)
  %r22 = add i64 0, 123
  ret i64 %r22
}

; Declarations of llvm intrinsics, may be unused
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg)
declare noalias i8* @malloc(i64 noundef)
declare void @free(i8* noundef)
declare i32 @puts(...)
