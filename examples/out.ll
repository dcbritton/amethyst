%struct.Matrix = type { i64**, i64 }
define dso_local void @Matrix.new$int(i64 noundef %r0, %struct.Matrix* noalias sret(%struct.Matrix) %r1) {
  ; Primitive parameter allocations and stores
  %r3 = alloca i64
  store i64 %r0, i64* %r3
  ; End parameter handling
  %r4 = load i64, i64* %r3
  %r5 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r1, i32 0, i32 1
  store i64 %r4, i64* %r5
  %r6 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r1, i32 0, i32 1
  %r7 = load i64, i64* %r6
  %r8 = call i64** @Matrix.allocateNxN$int(%struct.Matrix* %r1, i64 noundef %r7)
  %r9 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r1, i32 0, i32 0
  store i64** %r8, i64*** %r9
  ret void
}
define dso_local void @Matrix.new$Matrix(%struct.Matrix* noundef byval(%struct.Matrix) %r0, %struct.Matrix* noalias sret(%struct.Matrix) %r1) {
  ; Primitive parameter allocations and stores
  ; End parameter handling
  %r3 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r0, i32 0, i32 1 ; Getting ptr to member
  %r4 = load i64, i64* %r3
  %r5 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r1, i32 0, i32 1
  store i64 %r4, i64* %r5
  %r6 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r1, i32 0, i32 1
  %r7 = load i64, i64* %r6
  %r8 = call i64** @Matrix.allocateNxN$int(%struct.Matrix* %r1, i64 noundef %r7)
  %r9 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r1, i32 0, i32 0
  store i64** %r8, i64*** %r9

  ; Define i:int
  %r10 = alloca i64
  %r11 = add i64 0, 0
  store i64 %r11, i64* %r10
  ; End definition of i:int
  br label %cond0

cond0:
  ; Begin eq expr
  %r12 = load i64, i64* %r10
  %r13 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r1, i32 0, i32 1
  %r14 = load i64, i64* %r13
  %r15 = icmp ne i64 %r12, %r14
  ; End eq expr
  br i1 %r15, label %body0, label %exit0

body0:

  ; Define j:int
  %r16 = alloca i64
  %r17 = add i64 0, 0
  store i64 %r17, i64* %r16
  ; End definition of j:int
  br label %cond1

cond1:
  ; Begin eq expr
  %r18 = load i64, i64* %r16
  %r19 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r1, i32 0, i32 1
  %r20 = load i64, i64* %r19
  %r21 = icmp ne i64 %r18, %r20
  ; End eq expr
  br i1 %r21, label %body1, label %exit1

body1:
  %r22 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r0, i32 0, i32 0 ; Getting ptr to member
  %r23 = load i64**, i64*** %r22
  %r24 = load i64, i64* %r10
  %r25 = getelementptr i64*, i64** %r23, i64 %r24
  %r26 = load i64*, i64** %r25
  %r27 = load i64, i64* %r16
  %r28 = getelementptr i64, i64* %r26, i64 %r27
  %r29 = load i64, i64* %r28
  %r30 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r1, i32 0, i32 0
  %r31 = load i64**, i64*** %r30
  %r32 = load i64, i64* %r10
  %r33 = getelementptr i64*, i64** %r31, i64 %r32
  %r34 = load i64*, i64** %r33
  %r35 = load i64, i64* %r16
  %r36 = getelementptr i64, i64* %r34, i64 %r35
  store i64 %r29, i64* %r36
  ; Begin add expr
  %r37 = load i64, i64* %r16
  %r38 = add i64 0, 1
  %r39 = add i64 %r37, %r38
  ; End add expr
  store i64 %r39, i64* %r16
  br label %cond1

exit1:
  ; Begin add expr
  %r40 = load i64, i64* %r10
  %r41 = add i64 0, 1
  %r42 = add i64 %r40, %r41
  ; End add expr
  store i64 %r42, i64* %r10
  br label %cond0

exit0:
  ret void
}
define dso_local void @Matrix.new$Matrix.(%struct.Matrix* noundef %r0, %struct.Matrix* noalias sret(%struct.Matrix) %r1) {
  ; Primitive parameter allocations and stores
  %r3 = alloca %struct.Matrix*
  store %struct.Matrix* %r0, %struct.Matrix** %r3
  ; End parameter handling
  %r4 = load %struct.Matrix*, %struct.Matrix** %r3
  %r5 = add i64 0, 0
  %r6 = getelementptr %struct.Matrix, %struct.Matrix* %r4, i64 %r5
  %r7 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r6, i32 0, i32 1 ; Getting ptr to member
  %r8 = load i64, i64* %r7
  %r9 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r1, i32 0, i32 1
  store i64 %r8, i64* %r9
  %r10 = getelementptr i64*, i64** null, i32 1
  %r11 = ptrtoint i64** %r10 to i64
  %r12 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r1, i32 0, i32 1
  %r13 = load i64, i64* %r12
  %r14 = mul i64 %r11, %r13
  %r15 = call noalias i8* @malloc(i64 noundef %r14)
  %r16 = bitcast i8* %r15 to i64**
  %r17 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r1, i32 0, i32 0
  store i64** %r16, i64*** %r17

  ; Define i:int
  %r18 = alloca i64
  %r19 = add i64 0, 0
  store i64 %r19, i64* %r18
  ; End definition of i:int
  br label %cond0

cond0:
  ; Begin eq expr
  %r20 = load i64, i64* %r18
  %r21 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r1, i32 0, i32 1
  %r22 = load i64, i64* %r21
  %r23 = icmp ne i64 %r20, %r22
  ; End eq expr
  br i1 %r23, label %body0, label %exit0

body0:
  %r24 = getelementptr i64, i64* null, i32 1
  %r25 = ptrtoint i64* %r24 to i64
  %r26 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r1, i32 0, i32 1
  %r27 = load i64, i64* %r26
  %r28 = mul i64 %r25, %r27
  %r29 = call noalias i8* @malloc(i64 noundef %r28)
  %r30 = bitcast i8* %r29 to i64*
  %r31 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r1, i32 0, i32 0
  %r32 = load i64**, i64*** %r31
  %r33 = load i64, i64* %r18
  %r34 = getelementptr i64*, i64** %r32, i64 %r33
  store i64* %r30, i64** %r34
  ; Begin add expr
  %r35 = load i64, i64* %r18
  %r36 = add i64 0, 1
  %r37 = add i64 %r35, %r36
  ; End add expr
  store i64 %r37, i64* %r18
  br label %cond0

exit0:
  %r38 = add i64 0, 0
  store i64 %r38, i64* %r18
  br label %cond1

cond1:
  ; Begin eq expr
  %r39 = load i64, i64* %r18
  %r40 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r1, i32 0, i32 1
  %r41 = load i64, i64* %r40
  %r42 = icmp ne i64 %r39, %r41
  ; End eq expr
  br i1 %r42, label %body1, label %exit1

body1:

  ; Define j:int
  %r43 = alloca i64
  %r44 = add i64 0, 0
  store i64 %r44, i64* %r43
  ; End definition of j:int
  br label %cond2

cond2:
  ; Begin eq expr
  %r45 = load i64, i64* %r43
  %r46 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r1, i32 0, i32 1
  %r47 = load i64, i64* %r46
  %r48 = icmp ne i64 %r45, %r47
  ; End eq expr
  br i1 %r48, label %body2, label %exit2

body2:
  %r49 = load %struct.Matrix*, %struct.Matrix** %r3
  %r50 = add i64 0, 0
  %r51 = getelementptr %struct.Matrix, %struct.Matrix* %r49, i64 %r50
  %r52 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r51, i32 0, i32 0 ; Getting ptr to member
  %r53 = load i64**, i64*** %r52
  %r54 = load i64, i64* %r18
  %r55 = getelementptr i64*, i64** %r53, i64 %r54
  %r56 = load i64*, i64** %r55
  %r57 = load i64, i64* %r43
  %r58 = getelementptr i64, i64* %r56, i64 %r57
  %r59 = load i64, i64* %r58
  %r60 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r1, i32 0, i32 0
  %r61 = load i64**, i64*** %r60
  %r62 = load i64, i64* %r18
  %r63 = getelementptr i64*, i64** %r61, i64 %r62
  %r64 = load i64*, i64** %r63
  %r65 = load i64, i64* %r43
  %r66 = getelementptr i64, i64* %r64, i64 %r65
  store i64 %r59, i64* %r66
  ; Begin add expr
  %r67 = load i64, i64* %r43
  %r68 = add i64 0, 1
  %r69 = add i64 %r67, %r68
  ; End add expr
  store i64 %r69, i64* %r43
  br label %cond2

exit2:
  ; Begin add expr
  %r70 = load i64, i64* %r18
  %r71 = add i64 0, 1
  %r72 = add i64 %r70, %r71
  ; End add expr
  store i64 %r72, i64* %r18
  br label %cond1

exit1:
  ret void
}
define dso_local void @Matrix.setEntryAsIndexProduct(%struct.Matrix* %r0) {
  ; Primitive parameter allocations and stores
  ; End parameter handling

  ; Define i:int
  %r2 = alloca i64
  %r3 = add i64 0, 0
  store i64 %r3, i64* %r2
  ; End definition of i:int
  br label %cond0

cond0:
  ; Begin eq expr
  %r4 = load i64, i64* %r2
  %r5 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r0, i32 0, i32 1
  %r6 = load i64, i64* %r5
  %r7 = icmp ne i64 %r4, %r6
  ; End eq expr
  br i1 %r7, label %body0, label %exit0

body0:

  ; Define j:int
  %r8 = alloca i64
  %r9 = add i64 0, 0
  store i64 %r9, i64* %r8
  ; End definition of j:int
  br label %cond1

cond1:
  ; Begin eq expr
  %r10 = load i64, i64* %r8
  %r11 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r0, i32 0, i32 1
  %r12 = load i64, i64* %r11
  %r13 = icmp ne i64 %r10, %r12
  ; End eq expr
  br i1 %r13, label %body1, label %exit1

body1:
  ; Begin mult expr
  %r14 = load i64, i64* %r2
  %r15 = load i64, i64* %r8
  %r16 = mul i64 %r14, %r15
  ; End mult expr
  %r17 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r0, i32 0, i32 0
  %r18 = load i64**, i64*** %r17
  %r19 = load i64, i64* %r2
  %r20 = getelementptr i64*, i64** %r18, i64 %r19
  %r21 = load i64*, i64** %r20
  %r22 = load i64, i64* %r8
  %r23 = getelementptr i64, i64* %r21, i64 %r22
  store i64 %r16, i64* %r23
  ; Begin add expr
  %r24 = load i64, i64* %r8
  %r25 = add i64 0, 1
  %r26 = add i64 %r24, %r25
  ; End add expr
  store i64 %r26, i64* %r8
  br label %cond1

exit1:
  ; Begin add expr
  %r27 = load i64, i64* %r2
  %r28 = add i64 0, 1
  %r29 = add i64 %r27, %r28
  ; End add expr
  store i64 %r29, i64* %r2
  br label %cond0

exit0:
  ret void
}
define dso_local void @Matrix.deepCopy(%struct.Matrix* %r0, %struct.Matrix* noalias sret(%struct.Matrix) %r1) {
  ; Primitive parameter allocations and stores
  ; End parameter handling

  ; Define result:Matrix
  %r3 = alloca %struct.Matrix
  %r4 = alloca %struct.Matrix ; Allocation for sret result of new_expr
  %r5 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r0, i32 0, i32 1
  %r6 = load i64, i64* %r5
  call void @Matrix.new$int(i64 noundef %r6, %struct.Matrix* sret(%struct.Matrix) %r4)
  %r7 = getelementptr %struct.Matrix, %struct.Matrix* null, i32 1
  %r8 = ptrtoint %struct.Matrix* %r7 to i64
  %r9 = bitcast %struct.Matrix* %r3 to i8*
  %r10 = bitcast %struct.Matrix* %r4 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %r9, i8* %r10, i64 %r8, i1 false)
  ; End definition of result:Matrix

  ; Define i:int
  %r11 = alloca i64
  %r12 = add i64 0, 0
  store i64 %r12, i64* %r11
  ; End definition of i:int
  br label %cond0

cond0:
  ; Begin eq expr
  %r13 = load i64, i64* %r11
  %r14 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r0, i32 0, i32 1
  %r15 = load i64, i64* %r14
  %r16 = icmp ne i64 %r13, %r15
  ; End eq expr
  br i1 %r16, label %body0, label %exit0

body0:

  ; Define j:int
  %r17 = alloca i64
  %r18 = add i64 0, 0
  store i64 %r18, i64* %r17
  ; End definition of j:int
  br label %cond1

cond1:
  ; Begin eq expr
  %r19 = load i64, i64* %r17
  %r20 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r0, i32 0, i32 1
  %r21 = load i64, i64* %r20
  %r22 = icmp ne i64 %r19, %r21
  ; End eq expr
  br i1 %r22, label %body1, label %exit1

body1:
  %r23 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r0, i32 0, i32 0
  %r24 = load i64**, i64*** %r23
  %r25 = load i64, i64* %r11
  %r26 = getelementptr i64*, i64** %r24, i64 %r25
  %r27 = load i64*, i64** %r26
  %r28 = load i64, i64* %r17
  %r29 = getelementptr i64, i64* %r27, i64 %r28
  %r30 = load i64, i64* %r29
  %r31 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r3, i32 0, i32 0 ; Getting ptr to member
  %r32 = load i64**, i64*** %r31
  %r33 = load i64, i64* %r11
  %r34 = getelementptr i64*, i64** %r32, i64 %r33
  %r35 = load i64*, i64** %r34
  %r36 = load i64, i64* %r17
  %r37 = getelementptr i64, i64* %r35, i64 %r36
  store i64 %r30, i64* %r37
  ; Begin add expr
  %r38 = load i64, i64* %r17
  %r39 = add i64 0, 1
  %r40 = add i64 %r38, %r39
  ; End add expr
  store i64 %r40, i64* %r17
  br label %cond1

exit1:
  ; Begin add expr
  %r41 = load i64, i64* %r11
  %r42 = add i64 0, 1
  %r43 = add i64 %r41, %r42
  ; End add expr
  store i64 %r43, i64* %r11
  br label %cond0

exit0:
  %r44 = getelementptr %struct.Matrix, %struct.Matrix* null, i32 1
  %r45 = ptrtoint %struct.Matrix* %r44 to i64
  %r46 = bitcast %struct.Matrix* %r1 to i8*
  %r47 = bitcast %struct.Matrix* %r3 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %r46, i8* %r47, i64 %r45, i1 false)
  ret void
}
define dso_local i64** @Matrix.allocateNxN$int(%struct.Matrix* %r0, i64 noundef %r1) {
  ; Primitive parameter allocations and stores
  %r3 = alloca i64
  store i64 %r1, i64* %r3
  ; End parameter handling

  ; Define rows:int**
  %r4 = alloca i64**
  %r5 = getelementptr i64*, i64** null, i32 1
  %r6 = ptrtoint i64** %r5 to i64
  %r7 = load i64, i64* %r3
  %r8 = mul i64 %r6, %r7
  %r9 = call noalias i8* @malloc(i64 noundef %r8)
  %r10 = bitcast i8* %r9 to i64**
  store i64** %r10, i64*** %r4
  ; End definition of rows:int**

  ; Define i:int
  %r11 = alloca i64
  %r12 = add i64 0, 0
  store i64 %r12, i64* %r11
  ; End definition of i:int
  br label %cond0

cond0:
  ; Begin eq expr
  %r13 = load i64, i64* %r11
  %r14 = load i64, i64* %r3
  %r15 = icmp ne i64 %r13, %r14
  ; End eq expr
  br i1 %r15, label %body0, label %exit0

body0:
  %r16 = getelementptr i64, i64* null, i32 1
  %r17 = ptrtoint i64* %r16 to i64
  %r18 = load i64, i64* %r3
  %r19 = mul i64 %r17, %r18
  %r20 = call noalias i8* @malloc(i64 noundef %r19)
  %r21 = bitcast i8* %r20 to i64*
  %r22 = load i64**, i64*** %r4
  %r23 = load i64, i64* %r11
  %r24 = getelementptr i64*, i64** %r22, i64 %r23
  store i64* %r21, i64** %r24
  ; Begin add expr
  %r25 = load i64, i64* %r11
  %r26 = add i64 0, 1
  %r27 = add i64 %r25, %r26
  ; End add expr
  store i64 %r27, i64* %r11
  br label %cond0

exit0:
  %r28 = load i64**, i64*** %r4
  ret i64** %r28
}

@.global.mat = dso_local global %struct.Matrix zeroinitializer
define dso_local i64 @main() {
  ; Primitive parameter allocations and stores
  ; End parameter handling

  ; Define mat:Matrix
  %r1 = alloca %struct.Matrix
  %r2 = alloca %struct.Matrix ; Allocation for sret result of new_expr
  %r3 = add i64 0, 4
  call void @Matrix.new$int(i64 noundef %r3, %struct.Matrix* sret(%struct.Matrix) %r2)
  %r4 = getelementptr %struct.Matrix, %struct.Matrix* null, i32 1
  %r5 = ptrtoint %struct.Matrix* %r4 to i64
  %r6 = bitcast %struct.Matrix* %r1 to i8*
  %r7 = bitcast %struct.Matrix* %r2 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %r6, i8* %r7, i64 %r5, i1 false)
  ; End definition of mat:Matrix
  %r8 = add i64 0, 142
  %r9 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r1, i32 0, i32 0 ; Getting ptr to member
  %r10 = load i64**, i64*** %r9
  %r11 = add i64 0, 3
  %r12 = getelementptr i64*, i64** %r10, i64 %r11
  %r13 = load i64*, i64** %r12
  %r14 = add i64 0, 3
  %r15 = getelementptr i64, i64* %r13, i64 %r14
  store i64 %r8, i64* %r15
  %r16 = bitcast %struct.Matrix* @.global.mat to %struct.Matrix* ; Workaround to use globals in current register management system
  %r17 = getelementptr %struct.Matrix, %struct.Matrix* null, i32 1
  %r18 = ptrtoint %struct.Matrix* %r17 to i64
  %r19 = bitcast %struct.Matrix* %r16 to i8*
  %r20 = bitcast %struct.Matrix* %r1 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %r19, i8* %r20, i64 %r18, i1 false)
  %r21 = add i64 0, 255
  %r22 = bitcast %struct.Matrix* @.global.mat to %struct.Matrix* ; Workaround to use globals in current register management system
  %r23 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r22, i32 0, i32 0 ; Getting ptr to member
  %r24 = load i64**, i64*** %r23
  %r25 = add i64 0, 3
  %r26 = getelementptr i64*, i64** %r24, i64 %r25
  %r27 = load i64*, i64** %r26
  %r28 = add i64 0, 3
  %r29 = getelementptr i64, i64* %r27, i64 %r28
  store i64 %r21, i64* %r29
  %r30 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r1, i32 0, i32 0 ; Getting ptr to member
  %r31 = load i64**, i64*** %r30
  %r32 = add i64 0, 3
  %r33 = getelementptr i64*, i64** %r31, i64 %r32
  %r34 = load i64*, i64** %r33
  %r35 = add i64 0, 3
  %r36 = getelementptr i64, i64* %r34, i64 %r35
  %r37 = load i64, i64* %r36
  ret i64 %r37
}

; Declarations of llvm intrinsics, may be unused
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg)
declare noalias i8* @malloc(i64 noundef)
declare void @free(i8* noundef)
