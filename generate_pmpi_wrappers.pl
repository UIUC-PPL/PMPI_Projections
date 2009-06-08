#!/usr/bin/perl
#
# (c) 2009 Isaac Dooley
#

my %funcsToIgnore = ("MPI_Init" => true,  
		     "MPI_Initialized" => true,
		     "MPI_Finalize" => true,
		     "MPI_Pcontrol" => true,
		     "MPI_Unpack_external" => true,
		    );

my %funcsToInclude = (
		      "MPI_Send" => true,
		      "MPI_Recv" => true,
		      "MPI_Isend" => true,
		      "MPI_Irecv" => true,
		      "MPI_Wait" => true,
		      "MPI_Comm_rank" => true,
		      "MPI_Comm_size" => true,
		      "MPI_Barrier" => true,
		      "MPI_Reduce" => true,
		      "MPI_Allreduce" => true,
		      "MPI_Wtime" => true,
		      "" => true,
		      "" => true,
		      "" => true,

		     );


$definedEventIds = "";
$funcDefinitions = "";
$eventCounter = 0; 
$numFuncDefinitions = 0;

while ($line = <>){
  chomp $line;

  if($line =~ /(\S*) MPI_([^(]*)([^;]*)/){
    $returntype = $1;
    $func = "MPI_$2";
    $funcEvent = "MPI_${2}_event";
    $typedparams = $3;
    #print "$returntype $func   $typedparams\n";
#    print " func $func\n";

      # Create a version of the parameters with just their names, not the types.
      
      $untypedparams = $typedparams;
      
      # The order of these is important, as some share common prefixes
      $untypedparams =~ s/MPI_Request \*/ /g;
      $untypedparams =~ s/MPI_Comm_copy_attr_function \*/ /g;
      $untypedparams =~ s/MPI_Comm_errhandler_fn \*/ /g;
      $untypedparams =~ s/MPI_Errhandler \*/ /g;
      $untypedparams =~ s/MPI_Datatype \*/ /g;
      $untypedparams =~ s/MPI_File \*/ /g;
      $untypedparams =~ s/MPI_Group \*/ /g;
      $untypedparams =~ s/MPI_Offset \*/ /g;
      $untypedparams =~ s/MPI_Comm_delete_attr_function \*/ /g;
      $untypedparams =~ s/MPI_Win_delete_attr_function \*/ /g;
      $untypedparams =~ s/MPI_Win_copy_attr_function \*/ /g;
      $untypedparams =~ s/MPI_Type_delete_attr_function \*/ /g;
      $untypedparams =~ s/MPI_Type_copy_attr_function \*/ /g;
      $untypedparams =~ s/MPI_Handler_function \*/ /g;
      $untypedparams =~ s/MPI_Status \*/ /g;
      $untypedparams =~ s/MPI_Info \*/ /g;
      $untypedparams =~ s/MPI_Errhandler \*/ /g;
      $untypedparams =~ s/MPI_Grequest_query_function \*/ /g;
      $untypedparams =~ s/MPI_Grequest_free_function \*/ /g;
      $untypedparams =~ s/MPI_Grequest_cancel_function \*/ /g;
      $untypedparams =~ s/MPI_Comm_errhandler_fn \*/ /g;
      $untypedparams =~ s/MPI_File_errhandler_fn \*/ /g;
      $untypedparams =~ s/MPI_Comm_errhandler_fn \*/ /g;
      $untypedparams =~ s/MPI_Win_errhandler_fn \*/ /g;
      $untypedparams =~ s/MPI_Delete_function \*/ /g;
      $untypedparams =~ s/MPI_Copy_function \*/ /g;
      $untypedparams =~ s/MPI_Datarep_conversion_function \*/ /g;
      $untypedparams =~ s/MPI_Datarep_extent_function \*/ /g;
      $untypedparams =~ s/MPI_Op \*/ /g;
      $untypedparams =~ s/MPI_User_function \*/ /g;
      $untypedparams =~ s/MPI_Comm \*/ /g;
      $untypedparams =~ s/MPI_Win \*/ /g;
      $untypedparams =~ s/MPI_File \*/ /g;
      $untypedparams =~ s/MPI_Aint \*/ /g;

      
      $untypedparams =~ s/int \*/ /g;
      $untypedparams =~ s/void \*/ /g;
      $untypedparams =~ s/char \*\*\*/ /g;
      $untypedparams =~ s/char \*\*/ /g;
      $untypedparams =~ s/char \*/ /g;
      
      
      $untypedparams =~ s/MPI_Fint / /g;
      $untypedparams =~ s/MPI_Comm / /g;
      $untypedparams =~ s/MPI_Datatype / /g;
      $untypedparams =~ s/MPI_Aint / /g;
      $untypedparams =~ s/MPI_Op / /g;
      $untypedparams =~ s/MPI_Info / /g;
      $untypedparams =~ s/MPI_Win / /g;
      $untypedparams =~ s/MPI_Group / /g;
      $untypedparams =~ s/MPI_Errhandler / /g;
      $untypedparams =~ s/MPI_File / /g;
      $untypedparams =~ s/MPI_Offset / /g;
      $untypedparams =~ s/MPI_Request / /g;
      $untypedparams =~ s/MPI_F / /g;
      $untypedparams =~ s/MPI_Status / /g;

      $untypedparams =~ s/int / /g;      
      $untypedparams =~ s/void/ /g;
      
      $untypedparams =~ s/\[\]/ /g;
      $untypedparams =~ s/\[3\]/ /g;
      
      
      #    print "$untypedparams\n";
      
      
      
      $definedEventIds .= "#define $funcEvent $eventCounter\n";
      
      $stsEvents .= "EVENT $eventCounter $func \n";
      

    if (!( exists  $funcsToIgnore{ $func }) ) {

      $funcDefinitions .= "$returntype $func $typedparams {\n";
      $funcDefinitions .= "\twrite_END_PROCESSING();\n";
      $funcDefinitions .= "\tlong startTime = time_us();\n";
      $funcDefinitions .= "\t$returntype ret = P$func$untypedparams;\n";
      $funcDefinitions .= "\twrite_EVENT_PAIR($funcEvent, startTime);\n";
      
      $funcDefinitions .= "\twrite_BEGIN_PROCESSING();\n";
      $funcDefinitions .= "\treturn ret;\n";
      
      $funcDefinitions .= "}\n\n\n";

      $numFuncDefinitions ++;

    } else {
#      print " ignoring function $func\n";
    }
      
      $eventCounter++;
    }

#  if( $eventCounter == 176 ){
#    print "problem event $eventCounter is $func\n";
#  }
    
  
}



open (GENFILE, ">generated-eventids.h");
print GENFILE "$definedEventIds\n\n\n";
print GENFILE "#define NUM_EVENTS $eventCounter\n\n";


open (GENFILE, ">generated-definitions.C");
print GENFILE "#include \"PMPI_ProjectionsLogging.h\"\n";
print GENFILE "\n\n\n";
print GENFILE "$funcDefinitions";
print GENFILE "\n\n\n// Automatically generated by generate_pmpi_wrappers.pl\n";
print GENFILE "// This file contains $numFuncDefinitions wrapped mpi calls.\n";
print GENFILE "// PMPI_ProjectionsLogging.C wraps up two others (MPI_Init, MPI_Finalize)\n";

open (GENFILE, ">generated-stsEvents.txt");
print GENFILE "$stsEvents";

