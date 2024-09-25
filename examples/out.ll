; ModuleID = 'out.ll'
source_filename = "out.ll"

; Function Attrs: mustprogress nofree norecurse nosync nounwind readnone willreturn
define dso_local i32 @example(i32 noundef %0, i32 noundef %1) local_unnamed_addr #0 {
  %3 = mul i32 %0, 112323232
  %4 = mul i32 %3, %1
  ret i32 %4
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind readnone willreturn }
