@.global.a = dso_local global i64 zeroinitializer
@.global.b = dso_local global float zeroinitializer
%struct.Matrix = type { i64**, i64 }
define dso_local void @Matrix.new$int(i64 noundef %r0, %struct.Matrix* noalias sret(%struct.Matrix) %r1) {
  ; Primitive parameter allocations and stores
  %r3 = alloca i64
  store i64 %r0, i64* %r3
  ; End parameter handling
  %r4 = load i64, i64* %r3
  %r5 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r1, i32 0, i32 1
  store i64 %r4, i64* %r5
  %r6 = getelementptr i64*, i64** null, i32 1
  %r7 = ptrtoint i64** %r6 to i64
  %r8 = load i64, i64* %r3
  %r9 = mul i64 %r7, %r8
  %r10 = call noalias i8* @malloc(i64 noundef %r9)
  %r11 = bitcast i8* %r10 to i64**
  %r12 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r1, i32 0, i32 0
  store i64** %r11, i64*** %r12

  ; Define i:int
  %r13 = alloca i64
  %r14 = add i64 0, 0
  store i64 %r14, i64* %r13
  ; End definition of i:int
  br label %cond0

cond0:
  ; Begin eq expr
  %r15 = load i64, i64* %r13
  %r16 = load i64, i64* %r3
  %r17 = icmp ne i64 %r15, %r16
  ; End eq expr
  br i1 %r17, label %body0, label %exit0

body0:
  %r18 = getelementptr i64, i64* null, i32 1
  %r19 = ptrtoint i64* %r18 to i64
  %r20 = load i64, i64* %r3
  %r21 = mul i64 %r19, %r20
  %r22 = call noalias i8* @malloc(i64 noundef %r21)
  %r23 = bitcast i8* %r22 to i64*
  %r24 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r1, i32 0, i32 0
  %r25 = load i64**, i64*** %r24
  %r26 = load i64, i64* %r13
  %r27 = getelementptr i64*, i64** %r25, i64 %r26
  store i64* %r23, i64** %r27
  ; Begin add expr
  %r28 = load i64, i64* %r13
  %r29 = add i64 0, 1
  %r30 = add i64 %r28, %r29
  ; End add expr
  store i64 %r30, i64* %r13
  br label %cond0

exit0:
  ret void
}
define dso_local void @Matrix.new$Matrix(%struct.Matrix* noundef byval(%struct.Matrix) %r0, %struct.Matrix* noalias sret(%struct.Matrix) %r1) {
  ; Primitive parameter allocations and stores
  ; End parameter handling
  %r3 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r0, i32 0, i32 1 ; Getting ptr to member
  %r4 = load i64, i64* %r3
  %r5 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r1, i32 0, i32 1
  store i64 %r4, i64* %r5
  %r6 = getelementptr i64*, i64** null, i32 1
  %r7 = ptrtoint i64** %r6 to i64
  %r8 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r1, i32 0, i32 1
  %r9 = load i64, i64* %r8
  %r10 = mul i64 %r7, %r9
  %r11 = call noalias i8* @malloc(i64 noundef %r10)
  %r12 = bitcast i8* %r11 to i64**
  %r13 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r1, i32 0, i32 0
  store i64** %r12, i64*** %r13

  ; Define i:int
  %r14 = alloca i64
  %r15 = add i64 0, 0
  store i64 %r15, i64* %r14
  ; End definition of i:int
  br label %cond0

cond0:
  ; Begin eq expr
  %r16 = load i64, i64* %r14
  %r17 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r1, i32 0, i32 1
  %r18 = load i64, i64* %r17
  %r19 = icmp ne i64 %r16, %r18
  ; End eq expr
  br i1 %r19, label %body0, label %exit0

body0:
  %r20 = getelementptr i64, i64* null, i32 1
  %r21 = ptrtoint i64* %r20 to i64
  %r22 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r1, i32 0, i32 1
  %r23 = load i64, i64* %r22
  %r24 = mul i64 %r21, %r23
  %r25 = call noalias i8* @malloc(i64 noundef %r24)
  %r26 = bitcast i8* %r25 to i64*
  %r27 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r1, i32 0, i32 0
  %r28 = load i64**, i64*** %r27
  %r29 = load i64, i64* %r14
  %r30 = getelementptr i64*, i64** %r28, i64 %r29
  store i64* %r26, i64** %r30
  ; Begin add expr
  %r31 = load i64, i64* %r14
  %r32 = add i64 0, 1
  %r33 = add i64 %r31, %r32
  ; End add expr
  store i64 %r33, i64* %r14
  br label %cond0

exit0:
  %r34 = add i64 0, 0
  store i64 %r34, i64* %r14
  br label %cond1

cond1:
  ; Begin eq expr
  %r35 = load i64, i64* %r14
  %r36 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r1, i32 0, i32 1
  %r37 = load i64, i64* %r36
  %r38 = icmp ne i64 %r35, %r37
  ; End eq expr
  br i1 %r38, label %body1, label %exit1

body1:

  ; Define j:int
  %r39 = alloca i64
  %r40 = add i64 0, 0
  store i64 %r40, i64* %r39
  ; End definition of j:int
  br label %cond2

cond2:
  ; Begin eq expr
  %r41 = load i64, i64* %r39
  %r42 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r1, i32 0, i32 1
  %r43 = load i64, i64* %r42
  %r44 = icmp ne i64 %r41, %r43
  ; End eq expr
  br i1 %r44, label %body2, label %exit2

body2:
  %r45 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r0, i32 0, i32 0 ; Getting ptr to member
  %r46 = load i64**, i64*** %r45
  %r47 = load i64, i64* %r14
  %r48 = getelementptr i64*, i64** %r46, i64 %r47
  %r49 = load i64*, i64** %r48
  %r50 = load i64, i64* %r39
  %r51 = getelementptr i64, i64* %r49, i64 %r50
  %r52 = load i64, i64* %r51
  %r53 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r1, i32 0, i32 0
  %r54 = load i64**, i64*** %r53
  %r55 = load i64, i64* %r14
  %r56 = getelementptr i64*, i64** %r54, i64 %r55
  %r57 = load i64*, i64** %r56
  %r58 = load i64, i64* %r39
  %r59 = getelementptr i64, i64* %r57, i64 %r58
  store i64 %r52, i64* %r59
  ; Begin add expr
  %r60 = load i64, i64* %r39
  %r61 = add i64 0, 1
  %r62 = add i64 %r60, %r61
  ; End add expr
  store i64 %r62, i64* %r39
  br label %cond2

exit2:
  ; Begin add expr
  %r63 = load i64, i64* %r14
  %r64 = add i64 0, 1
  %r65 = add i64 %r63, %r64
  ; End add expr
  store i64 %r65, i64* %r14
  br label %cond1

exit1:
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

@.global.mat = dso_local global %struct.Matrix zeroinitializer
define dso_local i64 @foo$int$Matrix(i64 noundef %r0, %struct.Matrix* noundef byval(%struct.Matrix) %r1) {
  ; Primitive parameter allocations and stores
  %r3 = alloca i64
  store i64 %r0, i64* %r3
  ; End parameter handling
  ; Begin add expr
  %r4 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r1, i32 0, i32 1 ; Getting ptr to member
  %r5 = load i64, i64* %r4
  %r6 = load i64, i64* %r3
  %r7 = add i64 %r5, %r6
  ; End add expr
  ret i64 %r7
}

define dso_local void @bar$int$Matrix(i64 noundef %r0, %struct.Matrix* noundef byval(%struct.Matrix) %r1) {
  ; Primitive parameter allocations and stores
  %r3 = alloca i64
  store i64 %r0, i64* %r3
  ; End parameter handling
  ; Begin eq expr
  %r4 = load i64, i64* %r3
  %r5 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r1, i32 0, i32 1 ; Getting ptr to member
  %r6 = load i64, i64* %r5
  %r7 = icmp eq i64 %r4, %r6
  ; End eq expr
  br i1 %r7, label %ifbody0, label %exit0

ifbody0:

  ; Define j:int
  %r8 = alloca i64
  %r9 = load i64, i64* %r3
  store i64 %r9, i64* %r8
  ; End definition of j:int
  ret void
  br label %exit0

exit0:

  ; Define k:int
  %r10 = alloca i64
  %r11 = load i64, i64* %r3
  store i64 %r11, i64* %r10
  ; End definition of k:int
  ret void
}

define dso_local i64 @main() {
  ; Primitive parameter allocations and stores
  ; End parameter handling

  ; Define size:int
  %r1 = alloca i64
  %r2 = add i64 0, 12
  store i64 %r2, i64* %r1
  ; End definition of size:int

  ; Define mat:Matrix
  %r3 = alloca %struct.Matrix
  %r4 = alloca %struct.Matrix ; Placeholder allocating space for struct return
  %r5 = load i64, i64* %r1
  call void @Matrix.new$int(i64 noundef %r5, %struct.Matrix* sret(%struct.Matrix) %r4)
  %r6 = getelementptr %struct.Matrix, %struct.Matrix* null, i32 1
  %r7 = ptrtoint %struct.Matrix* %r6 to i64
  %r8 = bitcast %struct.Matrix* %r3 to i8*
  %r9 = bitcast %struct.Matrix* %r4 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %r8, i8* %r9, i64 %r7, i1 false)
  ; End definition of mat:Matrix

  ; Define i:int
  %r10 = alloca i64
  %r11 = add i64 0, 0
  store i64 %r11, i64* %r10
  ; End definition of i:int
  br label %cond0

cond0:
  ; Begin eq expr
  %r12 = load i64, i64* %r10
  %r13 = load i64, i64* %r1
  %r14 = icmp ne i64 %r12, %r13
  ; End eq expr
  br i1 %r14, label %body0, label %exit0

body0:

  ; Define j:int
  %r15 = alloca i64
  %r16 = add i64 0, 0
  store i64 %r16, i64* %r15
  ; End definition of j:int
  br label %cond1

cond1:
  ; Begin eq expr
  %r17 = load i64, i64* %r15
  %r18 = load i64, i64* %r1
  %r19 = icmp ne i64 %r17, %r18
  ; End eq expr
  br i1 %r19, label %body1, label %exit1

body1:
  ; Begin add expr
  ; Begin add expr
  %r20 = load i64, i64* %r10
  %r21 = load i64, i64* %r15
  %r22 = add i64 %r20, %r21
  ; End add expr
  %r23 = add i64 0, -1
  %r24 = add i64 %r22, %r23
  ; End add expr
  %r25 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r3, i32 0, i32 0 ; Getting ptr to member
  %r26 = load i64**, i64*** %r25
  %r27 = load i64, i64* %r10
  %r28 = getelementptr i64*, i64** %r26, i64 %r27
  %r29 = load i64*, i64** %r28
  %r30 = load i64, i64* %r15
  %r31 = getelementptr i64, i64* %r29, i64 %r30
  store i64 %r24, i64* %r31
  ; Begin add expr
  %r32 = load i64, i64* %r15
  %r33 = add i64 0, 1
  %r34 = add i64 %r32, %r33
  ; End add expr
  store i64 %r34, i64* %r15
  br label %cond1

exit1:
  ; Begin add expr
  %r35 = load i64, i64* %r10
  %r36 = add i64 0, 1
  %r37 = add i64 %r35, %r36
  ; End add expr
  store i64 %r37, i64* %r10
  br label %cond0

exit0:
  %r38 = alloca %struct.Matrix ; Placeholder allocating space for struct return
  call void @Matrix.new$Matrix(%struct.Matrix* noundef byval(%struct.Matrix) %r3, %struct.Matrix* sret(%struct.Matrix) %r38)
  %r39 = bitcast %struct.Matrix* @.global.mat to %struct.Matrix* ; Workaround to use globals in current register management system
  %r40 = getelementptr %struct.Matrix, %struct.Matrix* null, i32 1
  %r41 = ptrtoint %struct.Matrix* %r40 to i64
  %r42 = bitcast %struct.Matrix* %r39 to i8*
  %r43 = bitcast %struct.Matrix* %r38 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %r42, i8* %r43, i64 %r41, i1 false)
  %r44 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r3, i32 0, i32 1 ; Getting ptr to member
  %r45 = load i64, i64* %r44
  %r46 = bitcast i64* @.global.a to i64* ; Workaround to use globals in current register management system
  store i64 %r45, i64* %r46

  ; Define placeholder:int
  %r47 = alloca i64
  ; Begin add expr
  ; Begin add expr
  %r48 = bitcast i64* @.global.a to i64* ; Workaround to use globals in current register management system
  %r49 = load i64, i64* %r48
  %r50 = bitcast %struct.Matrix* @.global.mat to %struct.Matrix* ; Workaround to use globals in current register management system
  %r51 = call i64 @foo$int$Matrix(i64 noundef %r49, %struct.Matrix* noundef byval(%struct.Matrix) %r50)
  %r52 = getelementptr inbounds %struct.Matrix, %struct.Matrix* %r3, i32 0, i32 0 ; Getting ptr to member
  %r53 = load i64**, i64*** %r52
  ; Begin add expr
  %r54 = load i64, i64* %r1
  %r55 = add i64 0, 1
  %r56 = sub i64 %r54, %r55
  ; End add expr
  %r57 = getelementptr i64*, i64** %r53, i64 %r56
  %r58 = load i64*, i64** %r57
  ; Begin add expr
  %r59 = load i64, i64* %r1
  %r60 = add i64 0, 1
  %r61 = sub i64 %r59, %r60
  ; End add expr
  %r62 = getelementptr i64, i64* %r58, i64 %r61
  %r63 = load i64, i64* %r62
  %r64 = add i64 %r51, %r63
  ; End add expr
  %r65 = add i64 0, -123
  %r66 = sub i64 %r64, %r65
  ; End add expr
  store i64 %r66, i64* %r47
  ; End definition of placeholder:int

  ; Define k:int
  %r67 = alloca i64
  %r68 = add i64 0, 0
  store i64 %r68, i64* %r67
  ; End definition of k:int

  ; Define boolTest:bool
  %r69 = alloca i1
  %r70 = add i1 0, 1
  store i1 %r70, i1* %r69
  ; End definition of boolTest:bool
  br label %cond2

cond2:
  %r71 = load i1, i1* %r69
  br i1 %r71, label %body2, label %exit2

body2:
  ; Begin add expr
  %r72 = load i64, i64* %r67
  %r73 = add i64 0, 10
  %r74 = add i64 %r72, %r73
  ; End add expr
  store i64 %r74, i64* %r67
  ; Begin eq expr
  %r75 = load i64, i64* %r67
  %r76 = add i64 0, 100
  %r77 = icmp eq i64 %r75, %r76
  ; End eq expr
  br i1 %r77, label %ifbody3, label %elsifcond3x0

ifbody3:
  %r78 = add i64 0, 101
  ret i64 %r78
  br label %exit3

elsifcond3x0:
  ; Begin eq expr
  %r79 = load i64, i64* %r67
  %r80 = add i64 0, 90
  %r81 = icmp eq i64 %r79, %r80
  ; End eq expr
  br i1 %r81, label %elsifbody3x0, label %elsifcond3x1

elsifbody3x0:
  %r82 = add i64 0, 91
  ret i64 %r82
  br label %exit3

elsifcond3x1:
  ; Begin eq expr
  %r83 = load i64, i64* %r67
  %r84 = add i64 0, 80
  %r85 = icmp eq i64 %r83, %r84
  ; End eq expr
  br i1 %r85, label %elsifbody3x1, label %exit3

elsifbody3x1:
  br label %cond2 ; Break statement
  br label %exit2 ; Break statement
  br label %exit3

exit3:
  br label %cond2

exit2:
  %r86 = load i64, i64* %r67
  ret i64 %r86
}

; Declarations of llvm intrinstics, may be unused
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg)
declare noalias i8* @malloc(i64 noundef)
declare void @free(i8* noundef)
