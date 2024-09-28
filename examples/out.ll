define dso_local i32* @ptrFunc(i32* noundef %0) {
  ; Handle parameters
  %2 = alloca i32*
  store i32* %0, i32** %2

  ; Define a:int**
  %3 = alloca i32**
  %4 = alloca [3 x i32*]
  %5 = bitcast [3 x i32*]* %4 to i32**
  store i32** %5, i32*** %3
  ; End definition of a:int**
  %6 = load i32*, i32** %2
  ret i32* %6
}

define dso_local i32 @main() {
  ; Handle parameters
  %1 = add nsw i32 0, 0
  ret i32 %1
}

