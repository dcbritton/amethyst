%struct.DynamicIntArray = type { i64*, i64, i64 }
define dso_local void @DynamicIntArray.new(%struct.DynamicIntArray* noalias sret(%struct.DynamicIntArray) %r0) {
  ; Primitive parameter allocations and stores
  ; End parameter handling
  %r2 = add i64 0, 8
  %r3 = getelementptr inbounds %struct.DynamicIntArray, %struct.DynamicIntArray* %r0, i32 0, i32 2
  store i64 %r2, i64* %r3
  %r4 = getelementptr i64, i64* null, i32 1
  %r5 = ptrtoint i64* %r4 to i64
  %r6 = getelementptr inbounds %struct.DynamicIntArray, %struct.DynamicIntArray* %r0, i32 0, i32 2
  %r7 = load i64, i64* %r6
  %r8 = mul i64 %r5, %r7
  %r9 = call noalias i8* @malloc(i64 noundef %r8)
  %r10 = bitcast i8* %r9 to i64*
  %r11 = getelementptr inbounds %struct.DynamicIntArray, %struct.DynamicIntArray* %r0, i32 0, i32 0
  store i64* %r10, i64** %r11
  %r12 = add i64 0, 0
  %r13 = getelementptr inbounds %struct.DynamicIntArray, %struct.DynamicIntArray* %r0, i32 0, i32 1
  store i64 %r12, i64* %r13
  ret void
}
define dso_local void @DynamicIntArray.append$int(%struct.DynamicIntArray* %r0, i64 noundef %r1) {
  ; Primitive parameter allocations and stores
  %r3 = alloca i64
  store i64 %r1, i64* %r3
  ; End parameter handling
  ; Begin eq expr
  ; Begin add expr
  %r4 = getelementptr inbounds %struct.DynamicIntArray, %struct.DynamicIntArray* %r0, i32 0, i32 1
  %r5 = load i64, i64* %r4
  %r6 = add i64 0, 1
  %r7 = add i64 %r5, %r6
  ; End add expr
  %r8 = getelementptr inbounds %struct.DynamicIntArray, %struct.DynamicIntArray* %r0, i32 0, i32 2
  %r9 = load i64, i64* %r8
  %r10 = icmp eq i64 %r7, %r9
  ; End eq expr
  br i1 %r10, label %ifbody0, label %exit0

ifbody0:
  ; Begin mult expr
  %r11 = getelementptr inbounds %struct.DynamicIntArray, %struct.DynamicIntArray* %r0, i32 0, i32 2
  %r12 = load i64, i64* %r11
  %r13 = add i64 0, 2
  %r14 = mul i64 %r12, %r13
  ; End mult expr
  %r15 = getelementptr inbounds %struct.DynamicIntArray, %struct.DynamicIntArray* %r0, i32 0, i32 2
  store i64 %r14, i64* %r15

  ; Define tempData:int*
  %r16 = alloca i64*
  %r17 = getelementptr inbounds %struct.DynamicIntArray, %struct.DynamicIntArray* %r0, i32 0, i32 0
  %r18 = load i64*, i64** %r17
  store i64* %r18, i64** %r16
  ; End definition of tempData:int*
  %r19 = getelementptr i64, i64* null, i32 1
  %r20 = ptrtoint i64* %r19 to i64
  %r21 = getelementptr inbounds %struct.DynamicIntArray, %struct.DynamicIntArray* %r0, i32 0, i32 2
  %r22 = load i64, i64* %r21
  %r23 = mul i64 %r20, %r22
  %r24 = call noalias i8* @malloc(i64 noundef %r23)
  %r25 = bitcast i8* %r24 to i64*
  %r26 = getelementptr inbounds %struct.DynamicIntArray, %struct.DynamicIntArray* %r0, i32 0, i32 0
  store i64* %r25, i64** %r26

  ; Define i:int
  %r27 = alloca i64
  %r28 = add i64 0, 0
  store i64 %r28, i64* %r27
  ; End definition of i:int
  br label %cond1

cond1:
  ; Begin relational expr
  %r29 = load i64, i64* %r27
  %r30 = getelementptr inbounds %struct.DynamicIntArray, %struct.DynamicIntArray* %r0, i32 0, i32 1
  %r31 = load i64, i64* %r30
  %r32 = icmp slt i64 %r29, %r31
  ; End relational expr
  br i1 %r32, label %body1, label %exit1

body1:
  %r33 = load i64*, i64** %r16
  %r34 = load i64, i64* %r27
  %r35 = getelementptr i64, i64* %r33, i64 %r34
  %r36 = load i64, i64* %r35
  %r37 = getelementptr inbounds %struct.DynamicIntArray, %struct.DynamicIntArray* %r0, i32 0, i32 0
  %r38 = load i64*, i64** %r37
  %r39 = load i64, i64* %r27
  %r40 = getelementptr i64, i64* %r38, i64 %r39
  store i64 %r36, i64* %r40
  ; Begin add expr
  %r41 = load i64, i64* %r27
  %r42 = add i64 0, 1
  %r43 = add i64 %r41, %r42
  ; End add expr
  store i64 %r43, i64* %r27
  br label %cond1

exit1:
  %r44 = load i64*, i64** %r16
  %r45 = bitcast i64* %r44 to i8*
  call void @free(i8* noundef %r45)
  br label %exit0

exit0:
  %r46 = load i64, i64* %r3
  %r47 = getelementptr inbounds %struct.DynamicIntArray, %struct.DynamicIntArray* %r0, i32 0, i32 0
  %r48 = load i64*, i64** %r47
  %r49 = getelementptr inbounds %struct.DynamicIntArray, %struct.DynamicIntArray* %r0, i32 0, i32 1
  %r50 = load i64, i64* %r49
  %r51 = getelementptr i64, i64* %r48, i64 %r50
  store i64 %r46, i64* %r51
  ; Begin add expr
  %r52 = getelementptr inbounds %struct.DynamicIntArray, %struct.DynamicIntArray* %r0, i32 0, i32 1
  %r53 = load i64, i64* %r52
  %r54 = add i64 0, 1
  %r55 = add i64 %r53, %r54
  ; End add expr
  %r56 = getelementptr inbounds %struct.DynamicIntArray, %struct.DynamicIntArray* %r0, i32 0, i32 1
  store i64 %r55, i64* %r56
  ret void
}
define dso_local void @DynamicIntArray.destroy(%struct.DynamicIntArray* %r0) {
  ; Primitive parameter allocations and stores
  ; End parameter handling
  %r2 = getelementptr inbounds %struct.DynamicIntArray, %struct.DynamicIntArray* %r0, i32 0, i32 0
  %r3 = load i64*, i64** %r2
  %r4 = bitcast i64* %r3 to i8*
  call void @free(i8* noundef %r4)
  ret void
}

define dso_local i64 @main() {
  ; Primitive parameter allocations and stores
  ; End parameter handling

  ; Define arr:DynamicIntArray
  %r1 = alloca %struct.DynamicIntArray
  %r2 = alloca %struct.DynamicIntArray ; Allocation for sret result of new_expr
  call void @DynamicIntArray.new(%struct.DynamicIntArray* sret(%struct.DynamicIntArray) %r2)
  %r3 = getelementptr %struct.DynamicIntArray, %struct.DynamicIntArray* null, i32 1
  %r4 = ptrtoint %struct.DynamicIntArray* %r3 to i64
  %r5 = bitcast %struct.DynamicIntArray* %r1 to i8*
  %r6 = bitcast %struct.DynamicIntArray* %r2 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %r5, i8* %r6, i64 %r4, i1 false)
  ; End definition of arr:DynamicIntArray

  ; Define i:int
  %r7 = alloca i64
  %r8 = add i64 0, 0
  store i64 %r8, i64* %r7
  ; End definition of i:int
  br label %cond0

cond0:
  ; Begin relational expr
  %r9 = load i64, i64* %r7
  %r10 = add i64 0, 127
  %r11 = icmp slt i64 %r9, %r10
  ; End relational expr
  br i1 %r11, label %body0, label %exit0

body0:
  ; Begin eq expr
  ; Begin mult expr
  %r12 = load i64, i64* %r7
  %r13 = add i64 0, 2
  %r14 = srem i64 %r12, %r13
  ; End mult expr
  %r15 = add i64 0, 0
  %r16 = icmp eq i64 %r14, %r15
  ; End eq expr
  br i1 %r16, label %ifbody1, label %elsebody1

ifbody1:
  ; Begin mult expr
  %r17 = add i64 0, -1
  %r18 = load i64, i64* %r7
  %r19 = mul i64 %r17, %r18
  ; End mult expr
  call void @DynamicIntArray.append$int(%struct.DynamicIntArray* %r1, i64 noundef %r19)
  br label %exit1

elsebody1:
  %r20 = load i64, i64* %r7
  call void @DynamicIntArray.append$int(%struct.DynamicIntArray* %r1, i64 noundef %r20)
  br label %exit1

exit1:
  ; Begin add expr
  %r21 = load i64, i64* %r7
  %r22 = add i64 0, 1
  %r23 = add i64 %r21, %r22
  ; End add expr
  store i64 %r23, i64* %r7
  br label %cond0

exit0:

  ; Define str:char*
  %r24 = alloca i8*
  %r25 = alloca [4 x i8]
  %r26 = bitcast [4 x i8]* %r25 to i8*
  store i8* %r26, i8** %r24
  ; End definition of str:char*
  %r27 = add i8 0, 87 ; Char literal
  %r28 = load i8*, i8** %r24
  %r29 = add i64 0, 0
  %r30 = getelementptr i8, i8* %r28, i64 %r29
  store i8 %r27, i8* %r30
  %r31 = add i8 0, 79 ; Char literal
  %r32 = load i8*, i8** %r24
  %r33 = add i64 0, 1
  %r34 = getelementptr i8, i8* %r32, i64 %r33
  store i8 %r31, i8* %r34
  %r35 = add i8 0, 87 ; Char literal
  %r36 = load i8*, i8** %r24
  %r37 = add i64 0, 2
  %r38 = getelementptr i8, i8* %r36, i64 %r37
  store i8 %r35, i8* %r38
  %r39 = add i8 0, 0 ; Char literal
  %r40 = load i8*, i8** %r24
  %r41 = add i64 0, 3
  %r42 = getelementptr i8, i8* %r40, i64 %r41
  store i8 %r39, i8* %r42
  ; Begin eq expr
  %r43 = add i8 0, 87 ; Char literal
  %r44 = load i8*, i8** %r24
  %r45 = add i64 0, 0
  %r46 = getelementptr i8, i8* %r44, i64 %r45
  %r47 = load i8, i8* %r46
  %r48 = icmp eq i8 %r43, %r47
  ; End eq expr
  br i1 %r48, label %ifbody2, label %exit2

ifbody2:
  %r49 = add i64 0, 125
  ret i64 %r49
  br label %exit2

exit2:
  %r50 = getelementptr inbounds %struct.DynamicIntArray, %struct.DynamicIntArray* %r1, i32 0, i32 2 ; Getting ptr to member
  %r51 = load i64, i64* %r50
  ret i64 %r51
}

; Declarations of llvm intrinsics, may be unused
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg)
declare noalias i8* @malloc(i64 noundef)
declare void @free(i8* noundef)
