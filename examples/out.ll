%struct.Matrix = type { i64**, i64 }
define dso_local void @Matrix.new$int(i64 noundef %r0, %struct.Matrix* noalias sret(%struct.Matrix) %r1) {
  ; Primitive parameter allocations and stores
  %r3 = alloca i64
  store i64 %r0, i64* %r3
  ; End parameter handling
  %r4 = load i64, i64* %r3
  %r5 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r1, i32 0, i32 1
  store i64 %r4, i64* %r5
  ; Begin eq expr
  %r6 = load i64, i64* %r3
  %r7 = add i64 0, 0
  %r8 = icmp ne i64 %r6, %r7
  ; End eq expr
  br i1 %r8, label %ifbody0, label %exit0

ifbody0:
  %r9 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r1, i32 0, i32 1
  %r10 = load i64, i64* %r9
  %r11 = call i64** @Matrix.allocateNxN$int(%struct.Matrix* %r1, i64 noundef %r10)
  %r12 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r1, i32 0, i32 0
  store i64** %r11, i64*** %r12
  br label %exit0

exit0:
  ret void
}
define dso_local void @Matrix.setEntriesToIndexProduct(%struct.Matrix* %r0) {
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
define dso_local void @Matrix.destroy(%struct.Matrix* %r0) {
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
  %r8 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r0, i32 0, i32 0
  %r9 = load i64**, i64*** %r8
  %r10 = load i64, i64* %r2
  %r11 = getelementptr i64*, i64** %r9, i64 %r10
  %r12 = load i64*, i64** %r11
  %r13 = bitcast i64* %r12 to i8*
  call void @free(i8* noundef %r13)
  ; Begin add expr
  %r14 = load i64, i64* %r2
  %r15 = add i64 0, 1
  %r16 = add i64 %r14, %r15
  ; End add expr
  store i64 %r16, i64* %r2
  br label %cond0

exit0:
  %r17 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r0, i32 0, i32 0
  %r18 = load i64**, i64*** %r17
  %r19 = bitcast i64** %r18 to i8*
  call void @free(i8* noundef %r19)
  ret void
}
define dso_local void @Matrix.op.plus$Matrix(%struct.Matrix* %r0, %struct.Matrix* noundef byval(%struct.Matrix) %r1, %struct.Matrix* noalias sret(%struct.Matrix) %r2) {
  ; Primitive parameter allocations and stores
  ; End parameter handling
  ; Begin eq expr
  %r4 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r0, i32 0, i32 1
  %r5 = load i64, i64* %r4
  %r6 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r1, i32 0, i32 1 ; Getting ptr to member
  %r7 = load i64, i64* %r6
  %r8 = icmp ne i64 %r5, %r7
  ; End eq expr
  br i1 %r8, label %ifbody0, label %exit0

ifbody0:
  %r9 = alloca %struct.Matrix ; Allocation for sret result of new_expr
  %r10 = add i64 0, 0
  call void @Matrix.new$int(i64 noundef %r10, %struct.Matrix* sret(%struct.Matrix) %r9)
  %r11 = getelementptr %struct.Matrix, %struct.Matrix* null, i32 1
  %r12 = ptrtoint %struct.Matrix* %r11 to i64
  %r13 = bitcast %struct.Matrix* %r2 to i8*
  %r14 = bitcast %struct.Matrix* %r9 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %r13, i8* %r14, i64 %r12, i1 false)
  ret void
  br label %exit0

exit0:

  ; Define result:Matrix
  %r15 = alloca %struct.Matrix
  %r16 = alloca %struct.Matrix ; Allocation for sret result of internal '@' method call
  call void @Matrix.deepCopy(%struct.Matrix* %r0, %struct.Matrix* sret(%struct.Matrix) %r16)
  %r17 = getelementptr %struct.Matrix, %struct.Matrix* null, i32 1
  %r18 = ptrtoint %struct.Matrix* %r17 to i64
  %r19 = bitcast %struct.Matrix* %r15 to i8*
  %r20 = bitcast %struct.Matrix* %r16 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %r19, i8* %r20, i64 %r18, i1 false)
  ; End definition of result:Matrix

  ; Define i:int
  %r21 = alloca i64
  %r22 = add i64 0, 0
  store i64 %r22, i64* %r21
  ; End definition of i:int
  br label %cond1

cond1:
  ; Begin eq expr
  %r23 = load i64, i64* %r21
  %r24 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r0, i32 0, i32 1
  %r25 = load i64, i64* %r24
  %r26 = icmp ne i64 %r23, %r25
  ; End eq expr
  br i1 %r26, label %body1, label %exit1

body1:

  ; Define j:int
  %r27 = alloca i64
  %r28 = add i64 0, 0
  store i64 %r28, i64* %r27
  ; End definition of j:int
  br label %cond2

cond2:
  ; Begin eq expr
  %r29 = load i64, i64* %r27
  %r30 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r0, i32 0, i32 1
  %r31 = load i64, i64* %r30
  %r32 = icmp ne i64 %r29, %r31
  ; End eq expr
  br i1 %r32, label %body2, label %exit2

body2:
  ; Begin add expr
  %r33 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r15, i32 0, i32 0 ; Getting ptr to member
  %r34 = load i64**, i64*** %r33
  %r35 = load i64, i64* %r21
  %r36 = getelementptr i64*, i64** %r34, i64 %r35
  %r37 = load i64*, i64** %r36
  %r38 = load i64, i64* %r27
  %r39 = getelementptr i64, i64* %r37, i64 %r38
  %r40 = load i64, i64* %r39
  %r41 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r1, i32 0, i32 0 ; Getting ptr to member
  %r42 = load i64**, i64*** %r41
  %r43 = load i64, i64* %r21
  %r44 = getelementptr i64*, i64** %r42, i64 %r43
  %r45 = load i64*, i64** %r44
  %r46 = load i64, i64* %r27
  %r47 = getelementptr i64, i64* %r45, i64 %r46
  %r48 = load i64, i64* %r47
  %r49 = add i64 %r40, %r48
  ; End add expr
  %r50 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r15, i32 0, i32 0 ; Getting ptr to member
  %r51 = load i64**, i64*** %r50
  %r52 = load i64, i64* %r21
  %r53 = getelementptr i64*, i64** %r51, i64 %r52
  %r54 = load i64*, i64** %r53
  %r55 = load i64, i64* %r27
  %r56 = getelementptr i64, i64* %r54, i64 %r55
  store i64 %r49, i64* %r56
  ; Begin add expr
  %r57 = load i64, i64* %r27
  %r58 = add i64 0, 1
  %r59 = add i64 %r57, %r58
  ; End add expr
  store i64 %r59, i64* %r27
  br label %cond2

exit2:
  ; Begin add expr
  %r60 = load i64, i64* %r21
  %r61 = add i64 0, 1
  %r62 = add i64 %r60, %r61
  ; End add expr
  store i64 %r62, i64* %r21
  br label %cond1

exit1:
  %r63 = getelementptr %struct.Matrix, %struct.Matrix* null, i32 1
  %r64 = ptrtoint %struct.Matrix* %r63 to i64
  %r65 = bitcast %struct.Matrix* %r2 to i8*
  %r66 = bitcast %struct.Matrix* %r15 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %r65, i8* %r66, i64 %r64, i1 false)
  ret void
}
define dso_local void @Matrix.op.multiply$Matrix(%struct.Matrix* %r0, %struct.Matrix* noundef byval(%struct.Matrix) %r1, %struct.Matrix* noalias sret(%struct.Matrix) %r2) {
  ; Primitive parameter allocations and stores
  ; End parameter handling
  ; Begin eq expr
  %r4 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r0, i32 0, i32 1
  %r5 = load i64, i64* %r4
  %r6 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r1, i32 0, i32 1 ; Getting ptr to member
  %r7 = load i64, i64* %r6
  %r8 = icmp ne i64 %r5, %r7
  ; End eq expr
  br i1 %r8, label %ifbody0, label %exit0

ifbody0:
  %r9 = alloca %struct.Matrix ; Allocation for sret result of new_expr
  %r10 = add i64 0, 0
  call void @Matrix.new$int(i64 noundef %r10, %struct.Matrix* sret(%struct.Matrix) %r9)
  %r11 = getelementptr %struct.Matrix, %struct.Matrix* null, i32 1
  %r12 = ptrtoint %struct.Matrix* %r11 to i64
  %r13 = bitcast %struct.Matrix* %r2 to i8*
  %r14 = bitcast %struct.Matrix* %r9 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %r13, i8* %r14, i64 %r12, i1 false)
  ret void
  br label %exit0

exit0:

  ; Define result:Matrix
  %r15 = alloca %struct.Matrix
  %r16 = alloca %struct.Matrix ; Allocation for sret result of new_expr
  %r17 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r0, i32 0, i32 1
  %r18 = load i64, i64* %r17
  call void @Matrix.new$int(i64 noundef %r18, %struct.Matrix* sret(%struct.Matrix) %r16)
  %r19 = getelementptr %struct.Matrix, %struct.Matrix* null, i32 1
  %r20 = ptrtoint %struct.Matrix* %r19 to i64
  %r21 = bitcast %struct.Matrix* %r15 to i8*
  %r22 = bitcast %struct.Matrix* %r16 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %r21, i8* %r22, i64 %r20, i1 false)
  ; End definition of result:Matrix

  ; Define i:int
  %r23 = alloca i64
  %r24 = add i64 0, 0
  store i64 %r24, i64* %r23
  ; End definition of i:int
  br label %cond1

cond1:
  ; Begin eq expr
  %r25 = load i64, i64* %r23
  %r26 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r0, i32 0, i32 1
  %r27 = load i64, i64* %r26
  %r28 = icmp ne i64 %r25, %r27
  ; End eq expr
  br i1 %r28, label %body1, label %exit1

body1:

  ; Define j:int
  %r29 = alloca i64
  %r30 = add i64 0, 0
  store i64 %r30, i64* %r29
  ; End definition of j:int
  br label %cond2

cond2:
  ; Begin eq expr
  %r31 = load i64, i64* %r29
  %r32 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r0, i32 0, i32 1
  %r33 = load i64, i64* %r32
  %r34 = icmp ne i64 %r31, %r33
  ; End eq expr
  br i1 %r34, label %body2, label %exit2

body2:
  %r35 = add i64 0, 0
  %r36 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r15, i32 0, i32 0 ; Getting ptr to member
  %r37 = load i64**, i64*** %r36
  %r38 = load i64, i64* %r23
  %r39 = getelementptr i64*, i64** %r37, i64 %r38
  %r40 = load i64*, i64** %r39
  %r41 = load i64, i64* %r29
  %r42 = getelementptr i64, i64* %r40, i64 %r41
  store i64 %r35, i64* %r42

  ; Define k:int
  %r43 = alloca i64
  %r44 = add i64 0, 0
  store i64 %r44, i64* %r43
  ; End definition of k:int
  br label %cond3

cond3:
  ; Begin eq expr
  %r45 = load i64, i64* %r43
  %r46 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r0, i32 0, i32 1
  %r47 = load i64, i64* %r46
  %r48 = icmp ne i64 %r45, %r47
  ; End eq expr
  br i1 %r48, label %body3, label %exit3

body3:
  ; Begin add expr
  %r49 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r15, i32 0, i32 0 ; Getting ptr to member
  %r50 = load i64**, i64*** %r49
  %r51 = load i64, i64* %r23
  %r52 = getelementptr i64*, i64** %r50, i64 %r51
  %r53 = load i64*, i64** %r52
  %r54 = load i64, i64* %r29
  %r55 = getelementptr i64, i64* %r53, i64 %r54
  %r56 = load i64, i64* %r55
  ; Begin mult expr
  %r57 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r0, i32 0, i32 0
  %r58 = load i64**, i64*** %r57
  %r59 = load i64, i64* %r23
  %r60 = getelementptr i64*, i64** %r58, i64 %r59
  %r61 = load i64*, i64** %r60
  %r62 = load i64, i64* %r43
  %r63 = getelementptr i64, i64* %r61, i64 %r62
  %r64 = load i64, i64* %r63
  %r65 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r1, i32 0, i32 0 ; Getting ptr to member
  %r66 = load i64**, i64*** %r65
  %r67 = load i64, i64* %r43
  %r68 = getelementptr i64*, i64** %r66, i64 %r67
  %r69 = load i64*, i64** %r68
  %r70 = load i64, i64* %r29
  %r71 = getelementptr i64, i64* %r69, i64 %r70
  %r72 = load i64, i64* %r71
  %r73 = mul i64 %r64, %r72
  ; End mult expr
  %r74 = add i64 %r56, %r73
  ; End add expr
  %r75 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r15, i32 0, i32 0 ; Getting ptr to member
  %r76 = load i64**, i64*** %r75
  %r77 = load i64, i64* %r23
  %r78 = getelementptr i64*, i64** %r76, i64 %r77
  %r79 = load i64*, i64** %r78
  %r80 = load i64, i64* %r29
  %r81 = getelementptr i64, i64* %r79, i64 %r80
  store i64 %r74, i64* %r81
  ; Begin add expr
  %r82 = load i64, i64* %r43
  %r83 = add i64 0, 1
  %r84 = add i64 %r82, %r83
  ; End add expr
  store i64 %r84, i64* %r43
  br label %cond3

exit3:
  ; Begin add expr
  %r85 = load i64, i64* %r29
  %r86 = add i64 0, 1
  %r87 = add i64 %r85, %r86
  ; End add expr
  store i64 %r87, i64* %r29
  br label %cond2

exit2:
  ; Begin add expr
  %r88 = load i64, i64* %r23
  %r89 = add i64 0, 1
  %r90 = add i64 %r88, %r89
  ; End add expr
  store i64 %r90, i64* %r23
  br label %cond1

exit1:
  %r91 = getelementptr %struct.Matrix, %struct.Matrix* null, i32 1
  %r92 = ptrtoint %struct.Matrix* %r91 to i64
  %r93 = bitcast %struct.Matrix* %r2 to i8*
  %r94 = bitcast %struct.Matrix* %r15 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %r93, i8* %r94, i64 %r92, i1 false)
  ret void
}

@.global.mat = dso_local global %struct.Matrix zeroinitializer
define dso_local i64 @main() {
  ; Primitive parameter allocations and stores
  ; End parameter handling

  ; Define mat:Matrix
  %r1 = alloca %struct.Matrix
  %r2 = alloca %struct.Matrix ; Allocation for sret result of new_expr
  %r3 = add i64 0, 8
  call void @Matrix.new$int(i64 noundef %r3, %struct.Matrix* sret(%struct.Matrix) %r2)
  %r4 = getelementptr %struct.Matrix, %struct.Matrix* null, i32 1
  %r5 = ptrtoint %struct.Matrix* %r4 to i64
  %r6 = bitcast %struct.Matrix* %r1 to i8*
  %r7 = bitcast %struct.Matrix* %r2 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %r6, i8* %r7, i64 %r5, i1 false)
  ; End definition of mat:Matrix
  call void @Matrix.setEntriesToIndexProduct(%struct.Matrix* %r1)
  %r8 = alloca %struct.Matrix ; Allocation for sret result of internal '@' method call
  call void @Matrix.deepCopy(%struct.Matrix* %r1, %struct.Matrix* sret(%struct.Matrix) %r8)
  %r9 = bitcast %struct.Matrix* @.global.mat to %struct.Matrix* ; Workaround to use globals in current register management system
  %r10 = getelementptr %struct.Matrix, %struct.Matrix* null, i32 1
  %r11 = ptrtoint %struct.Matrix* %r10 to i64
  %r12 = bitcast %struct.Matrix* %r9 to i8*
  %r13 = bitcast %struct.Matrix* %r8 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %r12, i8* %r13, i64 %r11, i1 false)

  ; Define matSum:Matrix
  %r14 = alloca %struct.Matrix
  ; Begin add expr
  ; Begin mult expr
  %r15 = bitcast %struct.Matrix* @.global.mat to %struct.Matrix* ; Workaround to use globals in current register management system
  %r16 = alloca %struct.Matrix ; Allocation for sret result of user-defined operator
  call void @Matrix.op.multiply$Matrix(%struct.Matrix* %r1, %struct.Matrix* noundef byval(%struct.Matrix) %r15, %struct.Matrix* sret(%struct.Matrix) %r16)
  %r17 = alloca %struct.Matrix ; Allocation for sret result of user-defined operator
  call void @Matrix.op.plus$Matrix(%struct.Matrix* %r16, %struct.Matrix* noundef byval(%struct.Matrix) %r1, %struct.Matrix* sret(%struct.Matrix) %r17)
  %r18 = getelementptr %struct.Matrix, %struct.Matrix* null, i32 1
  %r19 = ptrtoint %struct.Matrix* %r18 to i64
  %r20 = bitcast %struct.Matrix* %r14 to i8*
  %r21 = bitcast %struct.Matrix* %r17 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %r20, i8* %r21, i64 %r19, i1 false)
  ; End definition of matSum:Matrix

  ; Define k:int
  %r22 = alloca i64
  %r23 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r14, i32 0, i32 0 ; Getting ptr to member
  %r24 = load i64**, i64*** %r23
  ; Begin add expr
  %r25 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r1, i32 0, i32 1 ; Getting ptr to member
  %r26 = load i64, i64* %r25
  %r27 = add i64 0, 1
  %r28 = sub i64 %r26, %r27
  ; End add expr
  %r29 = getelementptr i64*, i64** %r24, i64 %r28
  %r30 = load i64*, i64** %r29
  ; Begin add expr
  %r31 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r1, i32 0, i32 1 ; Getting ptr to member
  %r32 = load i64, i64* %r31
  %r33 = add i64 0, 1
  %r34 = sub i64 %r32, %r33
  ; End add expr
  %r35 = getelementptr i64, i64* %r30, i64 %r34
  %r36 = load i64, i64* %r35
  store i64 %r36, i64* %r22
  ; End definition of k:int
  call void @Matrix.destroy(%struct.Matrix* %r1)
  %r37 = bitcast %struct.Matrix* @.global.mat to %struct.Matrix* ; Workaround to use globals in current register management system
  call void @Matrix.destroy(%struct.Matrix* %r37)
  call void @Matrix.destroy(%struct.Matrix* %r14)

  ; Define a:float
  %r38 = alloca double
  %r39 = fadd double 0.0, 1.234
  store double %r39, double* %r38
  ; End definition of a:float

  ; Define b:float
  %r40 = alloca double
  %r41 = fadd double 0.0, 1.234
  store double %r41, double* %r40
  ; End definition of b:float
  ; Begin eq expr
  %r42 = load double, double* %r38
  %r43 = load double, double* %r40
  %r44 = fcmp oeq double %r42, %r43
  ; End eq expr
  br i1 %r44, label %ifbody0, label %elsebody0

ifbody0:
  ; Begin mult expr
  %r45 = add i64 0, 10
  %r46 = add i64 0, 4
  %r47 = srem i64 %r45, %r46
  ; End mult expr
  ret i64 %r47
  br label %exit0

elsebody0:
  ; Begin mult expr
  %r48 = add i64 0, 10
  %r49 = add i64 0, 3
  %r50 = sdiv i64 %r48, %r49
  ; End mult expr
  ret i64 %r50
  br label %exit0

exit0:
  %r51 = load i64, i64* %r22
  ret i64 %r51
}

; Declarations of llvm intrinsics, may be unused
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg)
declare noalias i8* @malloc(i64 noundef)
declare void @free(i8* noundef)
