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
define dso_local void @Matrix.setElements(%struct.Matrix* %r0) {
  ; Primitive parameter allocations and stores
  ; End parameter handling

  ; Define i:int
  %r2 = alloca i64
  %r3 = add i64 0, 0
  store i64 %r3, i64* %r2
  ; End definition of i:int
  br label %cond0

cond0:
  ; Begin relational expr
  %r4 = load i64, i64* %r2
  %r5 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r0, i32 0, i32 1
  %r6 = load i64, i64* %r5
  %r7 = icmp slt i64 %r4, %r6
  ; End relational expr
  br i1 %r7, label %body0, label %exit0

body0:

  ; Define j:int
  %r8 = alloca i64
  %r9 = add i64 0, 0
  store i64 %r9, i64* %r8
  ; End definition of j:int
  br label %cond1

cond1:
  ; Begin relational expr
  %r10 = load i64, i64* %r8
  %r11 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r0, i32 0, i32 1
  %r12 = load i64, i64* %r11
  %r13 = icmp slt i64 %r10, %r12
  ; End relational expr
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

  ; Define k:int
  %r35 = alloca i64
  %r36 = add i64 0, 0
  store i64 %r36, i64* %r35
  ; End definition of k:int
  br label %cond3

cond3:
  ; Begin eq expr
  %r37 = load i64, i64* %r35
  %r38 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r0, i32 0, i32 1
  %r39 = load i64, i64* %r38
  %r40 = icmp ne i64 %r37, %r39
  ; End eq expr
  br i1 %r40, label %body3, label %exit3

body3:
  ; Begin add expr
  %r41 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r15, i32 0, i32 0 ; Getting ptr to member
  %r42 = load i64**, i64*** %r41
  %r43 = load i64, i64* %r23
  %r44 = getelementptr i64*, i64** %r42, i64 %r43
  %r45 = load i64*, i64** %r44
  %r46 = load i64, i64* %r29
  %r47 = getelementptr i64, i64* %r45, i64 %r46
  %r48 = load i64, i64* %r47
  ; Begin mult expr
  %r49 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r0, i32 0, i32 0
  %r50 = load i64**, i64*** %r49
  %r51 = load i64, i64* %r23
  %r52 = getelementptr i64*, i64** %r50, i64 %r51
  %r53 = load i64*, i64** %r52
  %r54 = load i64, i64* %r35
  %r55 = getelementptr i64, i64* %r53, i64 %r54
  %r56 = load i64, i64* %r55
  %r57 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r1, i32 0, i32 0 ; Getting ptr to member
  %r58 = load i64**, i64*** %r57
  %r59 = load i64, i64* %r35
  %r60 = getelementptr i64*, i64** %r58, i64 %r59
  %r61 = load i64*, i64** %r60
  %r62 = load i64, i64* %r29
  %r63 = getelementptr i64, i64* %r61, i64 %r62
  %r64 = load i64, i64* %r63
  %r65 = mul i64 %r56, %r64
  ; End mult expr
  %r66 = add i64 %r48, %r65
  ; End add expr
  %r67 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r15, i32 0, i32 0 ; Getting ptr to member
  %r68 = load i64**, i64*** %r67
  %r69 = load i64, i64* %r23
  %r70 = getelementptr i64*, i64** %r68, i64 %r69
  %r71 = load i64*, i64** %r70
  %r72 = load i64, i64* %r29
  %r73 = getelementptr i64, i64* %r71, i64 %r72
  store i64 %r66, i64* %r73
  ; Begin add expr
  %r74 = load i64, i64* %r35
  %r75 = add i64 0, 1
  %r76 = add i64 %r74, %r75
  ; End add expr
  store i64 %r76, i64* %r35
  br label %cond3

exit3:
  ; Begin add expr
  %r77 = load i64, i64* %r29
  %r78 = add i64 0, 1
  %r79 = add i64 %r77, %r78
  ; End add expr
  store i64 %r79, i64* %r29
  br label %cond2

exit2:
  ; Begin add expr
  %r80 = load i64, i64* %r23
  %r81 = add i64 0, 1
  %r82 = add i64 %r80, %r81
  ; End add expr
  store i64 %r82, i64* %r23
  br label %cond1

exit1:
  %r83 = getelementptr %struct.Matrix, %struct.Matrix* null, i32 1
  %r84 = ptrtoint %struct.Matrix* %r83 to i64
  %r85 = bitcast %struct.Matrix* %r2 to i8*
  %r86 = bitcast %struct.Matrix* %r15 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %r85, i8* %r86, i64 %r84, i1 false)
  ret void
}

define dso_local i64 @main() {
  ; Primitive parameter allocations and stores
  ; End parameter handling

  ; Define m:Matrix
  %r1 = alloca %struct.Matrix
  %r2 = alloca %struct.Matrix ; Allocation for sret result of new_expr
  %r3 = add i64 0, 4
  call void @Matrix.new$int(i64 noundef %r3, %struct.Matrix* sret(%struct.Matrix) %r2)
  %r4 = getelementptr %struct.Matrix, %struct.Matrix* null, i32 1
  %r5 = ptrtoint %struct.Matrix* %r4 to i64
  %r6 = bitcast %struct.Matrix* %r1 to i8*
  %r7 = bitcast %struct.Matrix* %r2 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %r6, i8* %r7, i64 %r5, i1 false)
  ; End definition of m:Matrix
  call void @Matrix.setElements(%struct.Matrix* %r1)

  ; Define n:Matrix
  %r8 = alloca %struct.Matrix
  %r9 = alloca %struct.Matrix ; Allocation for sret result of internal '@' method call
  call void @Matrix.deepCopy(%struct.Matrix* %r1, %struct.Matrix* sret(%struct.Matrix) %r9)
  %r10 = getelementptr %struct.Matrix, %struct.Matrix* null, i32 1
  %r11 = ptrtoint %struct.Matrix* %r10 to i64
  %r12 = bitcast %struct.Matrix* %r8 to i8*
  %r13 = bitcast %struct.Matrix* %r9 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %r12, i8* %r13, i64 %r11, i1 false)
  ; End definition of n:Matrix

  ; Define p:Matrix
  %r14 = alloca %struct.Matrix
  ; Begin mult expr
  %r15 = alloca %struct.Matrix ; Allocation for sret result of user-defined operator
  call void @Matrix.op.multiply$Matrix(%struct.Matrix* %r1, %struct.Matrix* noundef byval(%struct.Matrix) %r8, %struct.Matrix* sret(%struct.Matrix) %r15)
  %r16 = getelementptr %struct.Matrix, %struct.Matrix* null, i32 1
  %r17 = ptrtoint %struct.Matrix* %r16 to i64
  %r18 = bitcast %struct.Matrix* %r14 to i8*
  %r19 = bitcast %struct.Matrix* %r15 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %r18, i8* %r19, i64 %r17, i1 false)
  ; End definition of p:Matrix

  ; Define space:char*
  %r20 = alloca i8*
  %r21 = alloca [2 x i8]
  %r22 = bitcast [2 x i8]* %r21 to i8*
  store i8* %r22, i8** %r20
  ; End definition of space:char*
  %r23 = add i8 0, 32 ; Char literal
  %r24 = load i8*, i8** %r20
  %r25 = add i64 0, 0
  %r26 = getelementptr i8, i8* %r24, i64 %r25
  store i8 %r23, i8* %r26
  %r27 = add i8 0, 0 ; Char literal
  %r28 = load i8*, i8** %r20
  %r29 = add i64 0, 1
  %r30 = getelementptr i8, i8* %r28, i64 %r29
  store i8 %r27, i8* %r30

  ; Define i:int
  %r31 = alloca i64
  %r32 = add i64 0, 0
  store i64 %r32, i64* %r31
  ; End definition of i:int
  br label %cond0

cond0:
  ; Begin relational expr
  %r33 = load i64, i64* %r31
  %r34 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r1, i32 0, i32 1 ; Getting ptr to member
  %r35 = load i64, i64* %r34
  %r36 = icmp slt i64 %r33, %r35
  ; End relational expr
  br i1 %r36, label %body0, label %exit0

body0:

  ; Define j:int
  %r37 = alloca i64
  %r38 = add i64 0, 0
  store i64 %r38, i64* %r37
  ; End definition of j:int
  br label %cond1

cond1:
  ; Begin relational expr
  %r39 = load i64, i64* %r37
  %r40 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r1, i32 0, i32 1 ; Getting ptr to member
  %r41 = load i64, i64* %r40
  %r42 = icmp slt i64 %r39, %r41
  ; End relational expr
  br i1 %r42, label %body1, label %exit1

body1:
  %r43 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r14, i32 0, i32 0 ; Getting ptr to member
  %r44 = load i64**, i64*** %r43
  %r45 = load i64, i64* %r31
  %r46 = getelementptr i64*, i64** %r44, i64 %r45
  %r47 = load i64*, i64** %r46
  %r48 = load i64, i64* %r37
  %r49 = getelementptr i64, i64* %r47, i64 %r48
  %r50 = load i64, i64* %r49
  %r51 = alloca [12 x i8] ; Buffer for the string
  %r52 = bitcast [12 x i8]* %r51 to i8*
  %r53 = call i32 (i8*, i8*, ...) @sprintf(i8* noundef %r52, i8* noundef getelementptr inbounds ([4 x i8], [4 x i8]* @.internal.sprintf.ld, i64 0, i64 0), i64 noundef %r50)
  %r54 = call i32 (i8*, ...) bitcast (i32 (...)* @puts to i32 (i8*, ...)*)(i8* noundef %r52)
  ; Begin add expr
  %r55 = load i64, i64* %r37
  %r56 = add i64 0, 1
  %r57 = add i64 %r55, %r56
  ; End add expr
  store i64 %r57, i64* %r37
  br label %cond1

exit1:
  ; Begin add expr
  %r58 = load i64, i64* %r31
  %r59 = add i64 0, 1
  %r60 = add i64 %r58, %r59
  ; End add expr
  store i64 %r60, i64* %r31
  br label %cond0

exit0:
  %r61 = add i64 0, 0
  ret i64 %r61
}

; Declarations of llvm intrinsics, may be unused
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg)
declare noalias i8* @malloc(i64 noundef)
declare void @free(i8* noundef)
declare i32 @puts(...)
@.internal.sprintf.ld = private unnamed_addr constant [4 x i8] c"%ld\00"
declare i32 @sprintf(i8* noundef, i8* noundef, ...)
