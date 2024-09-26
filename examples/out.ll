; ModuleID = 'out.ll'
source_filename = "out.ll"

; Function Attrs: mustprogress nofree norecurse nosync nounwind readnone willreturn
define dso_local i32** @ptrFunc(i32** noundef readnone returned %0) local_unnamed_addr #0 {
  ret i32** %0
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind readnone willreturn
define dso_local i32 @example(i32 noundef %0, i32 noundef %1) local_unnamed_addr #0 {
  %3 = mul i32 %0, 112323232
  %4 = mul i32 %3, %1
  ret i32 %4
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind readnone willreturn
define dso_local i32 @other(i32 noundef %0, i32 noundef %1) local_unnamed_addr #0 {
  %3 = mul i32 %0, 224646464
  %4 = mul i32 %3, %0
  %5 = mul i32 %4, %1
  ret i32 %5
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind readnone willreturn }
