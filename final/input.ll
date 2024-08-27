; ModuleID = 'mpi_example2.c'
source_filename = "mpi_example2.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%struct._comm = type opaque
%struct._dtype = type opaque
%struct._status = type { i32, i32, i32, i32 }

@lam_mpi_comm_world = external global %struct._comm, align 1
@lam_mpi_int = external global %struct._dtype, align 1
@.str = private unnamed_addr constant [29 x i8] c"Process 1 received data: %d\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main(i32 noundef %0, i8** noundef %1) #0 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i8**, align 8
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  %9 = alloca %struct._comm*, align 8
  %10 = alloca i32, align 4
  store i32 0, i32* %3, align 4
  store i32 %0, i32* %4, align 4
  store i8** %1, i8*** %5, align 8
  %11 = call i32 @MPI_Init(i32* noundef null, i8*** noundef null)
  %12 = call i32 @MPI_Comm_rank(%struct._comm* noundef @lam_mpi_comm_world, i32* noundef %6)
  %13 = load i32, i32* %6, align 4
  %14 = icmp eq i32 %13, 0
  br i1 %14, label %15, label %20

15:                                               ; preds = %2
  store i32 100, i32* %7, align 4
  %16 = bitcast i32* %7 to i8*
  %17 = call i32 @MPI_Send(i8* noundef %16, i32 noundef 1, %struct._dtype* noundef @lam_mpi_int, i32 noundef 1, i32 noundef 0, %struct._comm* noundef @lam_mpi_comm_world)
  %18 = bitcast i32* %7 to i8*
  %19 = call i32 @MPI_Send(i8* noundef %18, i32 noundef 1, %struct._dtype* noundef @lam_mpi_int, i32 noundef 2, i32 noundef 0, %struct._comm* noundef @lam_mpi_comm_world)
  br label %29

20:                                               ; preds = %2
  %21 = load i32, i32* %6, align 4
  %22 = icmp eq i32 %21, 1
  br i1 %22, label %23, label %28

23:                                               ; preds = %20
  %24 = bitcast i32* %8 to i8*
  %25 = call i32 @MPI_Recv(i8* noundef %24, i32 noundef 1, %struct._dtype* noundef @lam_mpi_int, i32 noundef 0, i32 noundef 0, %struct._comm* noundef @lam_mpi_comm_world, %struct._status* noundef null)
  %26 = load i32, i32* %8, align 4
  %27 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([29 x i8], [29 x i8]* @.str, i64 0, i64 0), i32 noundef %26)
  br label %28

28:                                               ; preds = %23, %20
  br label %29

29:                                               ; preds = %28, %15
  %30 = load i32, i32* %6, align 4
  %31 = srem i32 %30, 2
  %32 = load i32, i32* %6, align 4
  %33 = call i32 @MPI_Comm_split(%struct._comm* noundef @lam_mpi_comm_world, i32 noundef %31, i32 noundef %32, %struct._comm** noundef %9)
  %34 = load i32, i32* %6, align 4
  %35 = icmp eq i32 %34, 2
  br i1 %35, label %36, label %39

36:                                               ; preds = %29
  store i32 200, i32* %10, align 4
  %37 = bitcast i32* %10 to i8*
  %38 = call i32 @MPI_Recv(i8* noundef %37, i32 noundef 1, %struct._dtype* noundef @lam_mpi_int, i32 noundef 0, i32 noundef 0, %struct._comm* noundef @lam_mpi_comm_world, %struct._status* noundef null)
  br label %39

39:                                               ; preds = %36, %29
  %40 = call i32 @MPI_Finalize()
  ret i32 0
}

declare i32 @MPI_Init(i32* noundef, i8*** noundef) #1

declare i32 @MPI_Comm_rank(%struct._comm* noundef, i32* noundef) #1

declare i32 @MPI_Send(i8* noundef, i32 noundef, %struct._dtype* noundef, i32 noundef, i32 noundef, %struct._comm* noundef) #1

declare i32 @MPI_Recv(i8* noundef, i32 noundef, %struct._dtype* noundef, i32 noundef, i32 noundef, %struct._comm* noundef, %struct._status* noundef) #1

declare i32 @printf(i8* noundef, ...) #1

declare i32 @MPI_Comm_split(%struct._comm* noundef, i32 noundef, i32 noundef, %struct._comm** noundef) #1

declare i32 @MPI_Finalize() #1

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 1}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"Ubuntu clang version 14.0.0-1ubuntu1.1"}
