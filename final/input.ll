; ModuleID = 'mpi_example.c'
source_filename = "mpi_example.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%struct._comm = type opaque
%struct._dtype = type opaque
%struct._status = type { i32, i32, i32, i32 }

@lam_mpi_comm_world = external global %struct._comm, align 1
@lam_mpi_int = external global %struct._dtype, align 1
@.str = private unnamed_addr constant [29 x i8] c"Process 1 received data: %d\0A\00", align 1
@.str.1 = private unnamed_addr constant [29 x i8] c"Process 3 received data: %d\0A\00", align 1
@.str.2 = private unnamed_addr constant [29 x i8] c"Process 5 received data: %d\0A\00", align 1
@.str.3 = private unnamed_addr constant [29 x i8] c"Process 7 received data: %d\0A\00", align 1

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
  %11 = alloca i32, align 4
  %12 = alloca i32, align 4
  %13 = alloca i32, align 4
  %14 = alloca %struct._comm*, align 8
  %15 = alloca i32, align 4
  %16 = alloca i32, align 4
  store i32 0, i32* %3, align 4
  store i32 %0, i32* %4, align 4
  store i8** %1, i8*** %5, align 8
  %17 = call i32 @MPI_Init(i32* noundef null, i8*** noundef null)
  %18 = call i32 @MPI_Comm_rank(%struct._comm* noundef @lam_mpi_comm_world, i32* noundef %6)
  %19 = load i32, i32* %6, align 4
  %20 = icmp eq i32 %19, 0
  br i1 %20, label %21, label %24

21:                                               ; preds = %2
  store i32 100, i32* %7, align 4
  %22 = bitcast i32* %7 to i8*
  %23 = call i32 @MPI_Send(i8* noundef %22, i32 noundef 1, %struct._dtype* noundef @lam_mpi_int, i32 noundef 1, i32 noundef 0, %struct._comm* noundef @lam_mpi_comm_world)
  br label %33

24:                                               ; preds = %2
  %25 = load i32, i32* %6, align 4
  %26 = icmp eq i32 %25, 1
  br i1 %26, label %27, label %32

27:                                               ; preds = %24
  %28 = bitcast i32* %8 to i8*
  %29 = call i32 @MPI_Recv(i8* noundef %28, i32 noundef 1, %struct._dtype* noundef @lam_mpi_int, i32 noundef 0, i32 noundef 0, %struct._comm* noundef @lam_mpi_comm_world, %struct._status* noundef null)
  %30 = load i32, i32* %8, align 4
  %31 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([29 x i8], [29 x i8]* @.str, i64 0, i64 0), i32 noundef %30)
  br label %32

32:                                               ; preds = %27, %24
  br label %33

33:                                               ; preds = %32, %21
  %34 = load i32, i32* %6, align 4
  %35 = srem i32 %34, 2
  %36 = load i32, i32* %6, align 4
  %37 = call i32 @MPI_Comm_split(%struct._comm* noundef @lam_mpi_comm_world, i32 noundef %35, i32 noundef %36, %struct._comm** noundef %9)
  %38 = load i32, i32* %6, align 4
  %39 = icmp eq i32 %38, 2
  br i1 %39, label %40, label %44

40:                                               ; preds = %33
  store i32 200, i32* %10, align 4
  %41 = bitcast i32* %10 to i8*
  %42 = load %struct._comm*, %struct._comm** %9, align 8
  %43 = call i32 @MPI_Send(i8* noundef %41, i32 noundef 1, %struct._dtype* noundef @lam_mpi_int, i32 noundef 3, i32 noundef 1, %struct._comm* noundef %42)
  br label %54

44:                                               ; preds = %33
  %45 = load i32, i32* %6, align 4
  %46 = icmp eq i32 %45, 3
  br i1 %46, label %47, label %53

47:                                               ; preds = %44
  %48 = bitcast i32* %11 to i8*
  %49 = load %struct._comm*, %struct._comm** %9, align 8
  %50 = call i32 @MPI_Recv(i8* noundef %48, i32 noundef 1, %struct._dtype* noundef @lam_mpi_int, i32 noundef 2, i32 noundef 1, %struct._comm* noundef %49, %struct._status* noundef null)
  %51 = load i32, i32* %11, align 4
  %52 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([29 x i8], [29 x i8]* @.str.1, i64 0, i64 0), i32 noundef %51)
  br label %53

53:                                               ; preds = %47, %44
  br label %54

54:                                               ; preds = %53, %40
  %55 = load i32, i32* %6, align 4
  %56 = icmp eq i32 %55, 4
  br i1 %56, label %57, label %60

57:                                               ; preds = %54
  store i32 300, i32* %12, align 4
  %58 = bitcast i32* %12 to i8*
  %59 = call i32 @MPI_Send(i8* noundef %58, i32 noundef 1, %struct._dtype* noundef @lam_mpi_int, i32 noundef 5, i32 noundef 2, %struct._comm* noundef @lam_mpi_comm_world)
  br label %69

60:                                               ; preds = %54
  %61 = load i32, i32* %6, align 4
  %62 = icmp eq i32 %61, 5
  br i1 %62, label %63, label %68

63:                                               ; preds = %60
  %64 = bitcast i32* %13 to i8*
  %65 = call i32 @MPI_Recv(i8* noundef %64, i32 noundef 1, %struct._dtype* noundef @lam_mpi_int, i32 noundef 4, i32 noundef 2, %struct._comm* noundef @lam_mpi_comm_world, %struct._status* noundef null)
  %66 = load i32, i32* %13, align 4
  %67 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([29 x i8], [29 x i8]* @.str.2, i64 0, i64 0), i32 noundef %66)
  br label %68

68:                                               ; preds = %63, %60
  br label %69

69:                                               ; preds = %68, %57
  %70 = load i32, i32* %6, align 4
  %71 = srem i32 %70, 3
  %72 = load i32, i32* %6, align 4
  %73 = call i32 @MPI_Comm_split(%struct._comm* noundef @lam_mpi_comm_world, i32 noundef %71, i32 noundef %72, %struct._comm** noundef %14)
  %74 = load i32, i32* %6, align 4
  %75 = icmp eq i32 %74, 6
  br i1 %75, label %76, label %80

76:                                               ; preds = %69
  store i32 400, i32* %15, align 4
  %77 = bitcast i32* %15 to i8*
  %78 = load %struct._comm*, %struct._comm** %14, align 8
  %79 = call i32 @MPI_Send(i8* noundef %77, i32 noundef 1, %struct._dtype* noundef @lam_mpi_int, i32 noundef 7, i32 noundef 3, %struct._comm* noundef %78)
  br label %90

80:                                               ; preds = %69
  %81 = load i32, i32* %6, align 4
  %82 = icmp eq i32 %81, 7
  br i1 %82, label %83, label %89

83:                                               ; preds = %80
  %84 = bitcast i32* %16 to i8*
  %85 = load %struct._comm*, %struct._comm** %14, align 8
  %86 = call i32 @MPI_Recv(i8* noundef %84, i32 noundef 1, %struct._dtype* noundef @lam_mpi_int, i32 noundef 6, i32 noundef 3, %struct._comm* noundef %85, %struct._status* noundef null)
  %87 = load i32, i32* %16, align 4
  %88 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([29 x i8], [29 x i8]* @.str.3, i64 0, i64 0), i32 noundef %87)
  br label %89

89:                                               ; preds = %83, %80
  br label %90

90:                                               ; preds = %89, %76
  %91 = call i32 @MPI_Comm_free(%struct._comm** noundef %9)
  %92 = call i32 @MPI_Comm_free(%struct._comm** noundef %14)
  %93 = call i32 @MPI_Finalize()
  ret i32 0
}

declare i32 @MPI_Init(i32* noundef, i8*** noundef) #1

declare i32 @MPI_Comm_rank(%struct._comm* noundef, i32* noundef) #1

declare i32 @MPI_Send(i8* noundef, i32 noundef, %struct._dtype* noundef, i32 noundef, i32 noundef, %struct._comm* noundef) #1

declare i32 @MPI_Recv(i8* noundef, i32 noundef, %struct._dtype* noundef, i32 noundef, i32 noundef, %struct._comm* noundef, %struct._status* noundef) #1

declare i32 @printf(i8* noundef, ...) #1

declare i32 @MPI_Comm_split(%struct._comm* noundef, i32 noundef, i32 noundef, %struct._comm** noundef) #1

declare i32 @MPI_Comm_free(%struct._comm** noundef) #1

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
