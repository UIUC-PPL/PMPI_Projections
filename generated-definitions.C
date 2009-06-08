#include "PMPI_ProjectionsLogging.h"



int MPI_Abort (MPI_Comm comm, int errorcode) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Abort( comm,  errorcode);
	write_EVENT_PAIR(MPI_Abort_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Address (void *location, MPI_Aint *address) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Address( location,  address);
	write_EVENT_PAIR(MPI_Address_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Allgather (void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, MPI_Comm comm) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Allgather( sendbuf,  sendcount,  sendtype,  recvbuf,  recvcount,  recvtype,  comm);
	write_EVENT_PAIR(MPI_Allgather_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Allgatherv (void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int *recvcounts, int *displs, MPI_Datatype recvtype, MPI_Comm comm) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Allgatherv( sendbuf,  sendcount,  sendtype,  recvbuf,  recvcounts,  displs,  recvtype,  comm);
	write_EVENT_PAIR(MPI_Allgatherv_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Allreduce (void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Allreduce( sendbuf,  recvbuf,  count,  datatype,  op,  comm);
	write_EVENT_PAIR(MPI_Allreduce_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Alltoall (void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, MPI_Comm comm) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Alltoall( sendbuf,  sendcount,  sendtype,  recvbuf,  recvcount,  recvtype,  comm);
	write_EVENT_PAIR(MPI_Alltoall_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Alltoallv (void *sendbuf, int *sendcounts, int *sdispls, MPI_Datatype sendtype, void *recvbuf, int *recvcounts, int *rdispls, MPI_Datatype recvtype, MPI_Comm comm) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Alltoallv( sendbuf,  sendcounts,  sdispls,  sendtype,  recvbuf,  recvcounts,  rdispls,  recvtype,  comm);
	write_EVENT_PAIR(MPI_Alltoallv_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Barrier (MPI_Comm comm) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Barrier( comm);
	write_EVENT_PAIR(MPI_Barrier_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Bcast (void *buffer, int count, MPI_Datatype datatype,      int root, MPI_Comm comm) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Bcast( buffer,  count,  datatype,       root,  comm);
	write_EVENT_PAIR(MPI_Bcast_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Bsend (void *buf, int count, MPI_Datatype datatype,      int dest, int tag, MPI_Comm comm) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Bsend( buf,  count,  datatype,       dest,  tag,  comm);
	write_EVENT_PAIR(MPI_Bsend_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Bsend_init (void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Bsend_init( buf,  count,  datatype,  dest,  tag,  comm,  request);
	write_EVENT_PAIR(MPI_Bsend_init_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Buffer_attach (void *buffer, int size) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Buffer_attach( buffer,  size);
	write_EVENT_PAIR(MPI_Buffer_attach_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Buffer_detach (void *buffer, int *size) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Buffer_detach( buffer,  size);
	write_EVENT_PAIR(MPI_Buffer_detach_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Cancel (MPI_Request *request) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Cancel( request);
	write_EVENT_PAIR(MPI_Cancel_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Cart_coords (MPI_Comm comm, int rank, int maxdims, int *coords) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Cart_coords( comm,  rank,  maxdims,  coords);
	write_EVENT_PAIR(MPI_Cart_coords_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Cart_create (MPI_Comm old_comm, int ndims, int *dims, int *periods, int reorder, MPI_Comm *comm_cart) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Cart_create( old_comm,  ndims,  dims,  periods,  reorder,  comm_cart);
	write_EVENT_PAIR(MPI_Cart_create_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Cart_get (MPI_Comm comm, int maxdims, int *dims, int *periods, int *coords) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Cart_get( comm,  maxdims,  dims,  periods,  coords);
	write_EVENT_PAIR(MPI_Cart_get_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Cart_map (MPI_Comm comm, int ndims, int *dims, int *periods, int *newrank) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Cart_map( comm,  ndims,  dims,  periods,  newrank);
	write_EVENT_PAIR(MPI_Cart_map_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Cart_rank (MPI_Comm comm, int *coords, int *rank) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Cart_rank( comm,  coords,  rank);
	write_EVENT_PAIR(MPI_Cart_rank_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Cart_shift (MPI_Comm comm, int direction, int disp, int *rank_source, int *rank_dest) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Cart_shift( comm,  direction,  disp,  rank_source,  rank_dest);
	write_EVENT_PAIR(MPI_Cart_shift_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Cart_sub (MPI_Comm comm, int *remain_dims, MPI_Comm *new_comm) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Cart_sub( comm,  remain_dims,  new_comm);
	write_EVENT_PAIR(MPI_Cart_sub_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Cartdim_get (MPI_Comm comm, int *ndims) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Cartdim_get( comm,  ndims);
	write_EVENT_PAIR(MPI_Cartdim_get_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Comm_compare (MPI_Comm comm1, MPI_Comm comm2, int *result) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Comm_compare( comm1,  comm2,  result);
	write_EVENT_PAIR(MPI_Comm_compare_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Comm_create (MPI_Comm comm, MPI_Group group, MPI_Comm *newcomm) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Comm_create( comm,  group,  newcomm);
	write_EVENT_PAIR(MPI_Comm_create_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Comm_dup (MPI_Comm comm, MPI_Comm *newcomm) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Comm_dup( comm,  newcomm);
	write_EVENT_PAIR(MPI_Comm_dup_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Comm_free (MPI_Comm *comm) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Comm_free( comm);
	write_EVENT_PAIR(MPI_Comm_free_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Comm_get_name (MPI_Comm comm, char *comm_name, int *resultlen) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Comm_get_name( comm,  comm_name,  resultlen);
	write_EVENT_PAIR(MPI_Comm_get_name_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Comm_group (MPI_Comm comm, MPI_Group *group) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Comm_group( comm,  group);
	write_EVENT_PAIR(MPI_Comm_group_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Comm_rank (MPI_Comm comm, int *rank) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Comm_rank( comm,  rank);
	write_EVENT_PAIR(MPI_Comm_rank_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Comm_remote_group (MPI_Comm comm, MPI_Group *group) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Comm_remote_group( comm,  group);
	write_EVENT_PAIR(MPI_Comm_remote_group_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Comm_remote_size (MPI_Comm comm, int *size) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Comm_remote_size( comm,  size);
	write_EVENT_PAIR(MPI_Comm_remote_size_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Comm_size (MPI_Comm comm, int *size) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Comm_size( comm,  size);
	write_EVENT_PAIR(MPI_Comm_size_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Comm_split (MPI_Comm comm, int color, int key, MPI_Comm *newcomm) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Comm_split( comm,  color,  key,  newcomm);
	write_EVENT_PAIR(MPI_Comm_split_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Comm_test_inter (MPI_Comm comm, int *flag) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Comm_test_inter( comm,  flag);
	write_EVENT_PAIR(MPI_Comm_test_inter_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Dims_create (int nnodes, int ndims, int *dims) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Dims_create( nnodes,  ndims,  dims);
	write_EVENT_PAIR(MPI_Dims_create_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Errhandler_create (MPI_Handler_function *function,  MPI_Errhandler *errhandler) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Errhandler_create( function,   errhandler);
	write_EVENT_PAIR(MPI_Errhandler_create_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Errhandler_free (MPI_Errhandler *errhandler) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Errhandler_free( errhandler);
	write_EVENT_PAIR(MPI_Errhandler_free_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Errhandler_get (MPI_Comm comm, MPI_Errhandler *errhandler) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Errhandler_get( comm,  errhandler);
	write_EVENT_PAIR(MPI_Errhandler_get_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Error_class (int errorcode, int *errorclass) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Error_class( errorcode,  errorclass);
	write_EVENT_PAIR(MPI_Error_class_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Error_string (int errorcode, char *string, int *resultlen) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Error_string( errorcode,  string,  resultlen);
	write_EVENT_PAIR(MPI_Error_string_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_File_set_errhandler ( MPI_File file, MPI_Errhandler errhandler) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_File_set_errhandler(  file,  errhandler);
	write_EVENT_PAIR(MPI_File_set_errhandler_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_File_get_errhandler ( MPI_File file, MPI_Errhandler *errhandler) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_File_get_errhandler(  file,  errhandler);
	write_EVENT_PAIR(MPI_File_get_errhandler_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_File_open (MPI_Comm comm, char *filename, int amode, MPI_Info info, MPI_File *fh) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_File_open( comm,  filename,  amode,  info,  fh);
	write_EVENT_PAIR(MPI_File_open_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_File_close (MPI_File *fh) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_File_close( fh);
	write_EVENT_PAIR(MPI_File_close_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_File_delete (char *filename, MPI_Info info) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_File_delete( filename,  info);
	write_EVENT_PAIR(MPI_File_delete_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_File_set_size (MPI_File fh, MPI_Offset size) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_File_set_size( fh,  size);
	write_EVENT_PAIR(MPI_File_set_size_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_File_preallocate (MPI_File fh, MPI_Offset size) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_File_preallocate( fh,  size);
	write_EVENT_PAIR(MPI_File_preallocate_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_File_get_size (MPI_File fh, MPI_Offset *size) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_File_get_size( fh,  size);
	write_EVENT_PAIR(MPI_File_get_size_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_File_get_group (MPI_File fh, MPI_Group *group) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_File_get_group( fh,  group);
	write_EVENT_PAIR(MPI_File_get_group_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_File_get_amode (MPI_File fh, int *amode) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_File_get_amode( fh,  amode);
	write_EVENT_PAIR(MPI_File_get_amode_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_File_set_info (MPI_File fh, MPI_Info info) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_File_set_info( fh,  info);
	write_EVENT_PAIR(MPI_File_set_info_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_File_get_info (MPI_File fh, MPI_Info *info_used) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_File_get_info( fh,  info_used);
	write_EVENT_PAIR(MPI_File_get_info_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_File_set_view (MPI_File fh, MPI_Offset disp, MPI_Datatype etype, MPI_Datatype filetype, char *datarep, MPI_Info info) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_File_set_view( fh,  disp,  etype,  filetype,  datarep,  info);
	write_EVENT_PAIR(MPI_File_set_view_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_File_get_view (MPI_File fh, MPI_Offset *disp, MPI_Datatype *etype, MPI_Datatype *filetype, char *datarep) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_File_get_view( fh,  disp,  etype,  filetype,  datarep);
	write_EVENT_PAIR(MPI_File_get_view_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_File_read_at (MPI_File fh, MPI_Offset offset, void *buf, int count, MPI_Datatype datatype, MPI_Status *status) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_File_read_at( fh,  offset,  buf,  count,  datatype,  status);
	write_EVENT_PAIR(MPI_File_read_at_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_File_read_at_all (MPI_File fh, MPI_Offset offset, void *buf,  int count, MPI_Datatype datatype, MPI_Status *status) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_File_read_at_all( fh,  offset,  buf,   count,  datatype,  status);
	write_EVENT_PAIR(MPI_File_read_at_all_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_File_write_at (MPI_File fh, MPI_Offset offset, void *buf, int count, MPI_Datatype datatype, MPI_Status *status) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_File_write_at( fh,  offset,  buf,  count,  datatype,  status);
	write_EVENT_PAIR(MPI_File_write_at_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_File_write_at_all (MPI_File fh, MPI_Offset offset, void *buf,  int count, MPI_Datatype datatype, MPI_Status *status) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_File_write_at_all( fh,  offset,  buf,   count,  datatype,  status);
	write_EVENT_PAIR(MPI_File_write_at_all_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_File_read (MPI_File fh, void *buf, int count, MPI_Datatype datatype, MPI_Status *status) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_File_read( fh,  buf,  count,  datatype,  status);
	write_EVENT_PAIR(MPI_File_read_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_File_read_all (MPI_File fh, void *buf, int count, MPI_Datatype datatype, MPI_Status *status) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_File_read_all( fh,  buf,  count,  datatype,  status);
	write_EVENT_PAIR(MPI_File_read_all_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_File_write (MPI_File fh, void *buf, int count, MPI_Datatype datatype, MPI_Status *status) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_File_write( fh,  buf,  count,  datatype,  status);
	write_EVENT_PAIR(MPI_File_write_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_File_write_all (MPI_File fh, void *buf, int count, MPI_Datatype datatype, MPI_Status *status) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_File_write_all( fh,  buf,  count,  datatype,  status);
	write_EVENT_PAIR(MPI_File_write_all_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_File_seek (MPI_File fh, MPI_Offset offset, int whence) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_File_seek( fh,  offset,  whence);
	write_EVENT_PAIR(MPI_File_seek_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_File_get_position (MPI_File fh, MPI_Offset *offset) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_File_get_position( fh,  offset);
	write_EVENT_PAIR(MPI_File_get_position_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_File_get_byte_offset (MPI_File fh, MPI_Offset offset,   MPI_Offset *disp) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_File_get_byte_offset( fh,  offset,    disp);
	write_EVENT_PAIR(MPI_File_get_byte_offset_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_File_read_shared (MPI_File fh, void *buf, int count,  MPI_Datatype datatype, MPI_Status *status) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_File_read_shared( fh,  buf,  count,   datatype,  status);
	write_EVENT_PAIR(MPI_File_read_shared_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_File_write_shared (MPI_File fh, void *buf, int count,  MPI_Datatype datatype, MPI_Status *status) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_File_write_shared( fh,  buf,  count,   datatype,  status);
	write_EVENT_PAIR(MPI_File_write_shared_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_File_read_ordered (MPI_File fh, void *buf, int count,  MPI_Datatype datatype, MPI_Status *status) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_File_read_ordered( fh,  buf,  count,   datatype,  status);
	write_EVENT_PAIR(MPI_File_read_ordered_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_File_write_ordered (MPI_File fh, void *buf, int count,  MPI_Datatype datatype, MPI_Status *status) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_File_write_ordered( fh,  buf,  count,   datatype,  status);
	write_EVENT_PAIR(MPI_File_write_ordered_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_File_seek_shared (MPI_File fh, MPI_Offset offset, int whence) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_File_seek_shared( fh,  offset,  whence);
	write_EVENT_PAIR(MPI_File_seek_shared_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_File_get_position_shared (MPI_File fh, MPI_Offset *offset) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_File_get_position_shared( fh,  offset);
	write_EVENT_PAIR(MPI_File_get_position_shared_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_File_read_at_all_begin (MPI_File fh, MPI_Offset offset, void *buf,   int count, MPI_Datatype datatype) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_File_read_at_all_begin( fh,  offset,  buf,    count,  datatype);
	write_EVENT_PAIR(MPI_File_read_at_all_begin_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_File_read_at_all_end (MPI_File fh, void *buf, MPI_Status *status) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_File_read_at_all_end( fh,  buf,  status);
	write_EVENT_PAIR(MPI_File_read_at_all_end_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_File_write_at_all_begin (MPI_File fh, MPI_Offset offset, void *buf,   int count, MPI_Datatype datatype) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_File_write_at_all_begin( fh,  offset,  buf,    count,  datatype);
	write_EVENT_PAIR(MPI_File_write_at_all_begin_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_File_write_at_all_end (MPI_File fh, void *buf, MPI_Status *status) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_File_write_at_all_end( fh,  buf,  status);
	write_EVENT_PAIR(MPI_File_write_at_all_end_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_File_read_all_begin (MPI_File fh, void *buf, int count,  MPI_Datatype datatype) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_File_read_all_begin( fh,  buf,  count,   datatype);
	write_EVENT_PAIR(MPI_File_read_all_begin_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_File_read_all_end (MPI_File fh, void *buf, MPI_Status *status) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_File_read_all_end( fh,  buf,  status);
	write_EVENT_PAIR(MPI_File_read_all_end_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_File_write_all_begin (MPI_File fh, void *buf, int count,   MPI_Datatype datatype) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_File_write_all_begin( fh,  buf,  count,    datatype);
	write_EVENT_PAIR(MPI_File_write_all_begin_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_File_write_all_end (MPI_File fh, void *buf, MPI_Status *status) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_File_write_all_end( fh,  buf,  status);
	write_EVENT_PAIR(MPI_File_write_all_end_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_File_read_ordered_begin (MPI_File fh, void *buf, int count,   MPI_Datatype datatype) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_File_read_ordered_begin( fh,  buf,  count,    datatype);
	write_EVENT_PAIR(MPI_File_read_ordered_begin_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_File_read_ordered_end (MPI_File fh, void *buf, MPI_Status *status) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_File_read_ordered_end( fh,  buf,  status);
	write_EVENT_PAIR(MPI_File_read_ordered_end_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_File_write_ordered_begin (MPI_File fh, void *buf, int count,    MPI_Datatype datatype) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_File_write_ordered_begin( fh,  buf,  count,     datatype);
	write_EVENT_PAIR(MPI_File_write_ordered_begin_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_File_write_ordered_end (MPI_File fh, void *buf, MPI_Status *status) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_File_write_ordered_end( fh,  buf,  status);
	write_EVENT_PAIR(MPI_File_write_ordered_end_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_File_get_type_extent (MPI_File fh, MPI_Datatype datatype,   MPI_Aint *extent) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_File_get_type_extent( fh,  datatype,    extent);
	write_EVENT_PAIR(MPI_File_get_type_extent_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_File_set_atomicity (MPI_File fh, int flag) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_File_set_atomicity( fh,  flag);
	write_EVENT_PAIR(MPI_File_set_atomicity_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_File_get_atomicity (MPI_File fh, int *flag) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_File_get_atomicity( fh,  flag);
	write_EVENT_PAIR(MPI_File_get_atomicity_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_File_sync (MPI_File fh) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_File_sync( fh);
	write_EVENT_PAIR(MPI_File_sync_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Gather (void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Gather( sendbuf,  sendcount,  sendtype,  recvbuf,  recvcount,  recvtype,  root,  comm);
	write_EVENT_PAIR(MPI_Gather_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Gatherv (void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int *recvcounts, int *displs, MPI_Datatype recvtype, int root, MPI_Comm comm) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Gatherv( sendbuf,  sendcount,  sendtype,  recvbuf,  recvcounts,  displs,  recvtype,  root,  comm);
	write_EVENT_PAIR(MPI_Gatherv_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Get_count (MPI_Status *status, MPI_Datatype datatype, int *count) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Get_count( status,  datatype,  count);
	write_EVENT_PAIR(MPI_Get_count_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Get_elements (MPI_Status *status, MPI_Datatype datatype, int *count) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Get_elements( status,  datatype,  count);
	write_EVENT_PAIR(MPI_Get_elements_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Get_processor_name (char *name, int *resultlen) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Get_processor_name( name,  resultlen);
	write_EVENT_PAIR(MPI_Get_processor_name_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Get_version (int *version, int *subversion) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Get_version( version,  subversion);
	write_EVENT_PAIR(MPI_Get_version_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Graph_create (MPI_Comm comm_old, int nnodes, int *index, int *edges, int reorder, MPI_Comm *comm_graph) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Graph_create( comm_old,  nnodes,  index,  edges,  reorder,  comm_graph);
	write_EVENT_PAIR(MPI_Graph_create_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Graph_get (MPI_Comm comm, int maxindex, int maxedges, int *index, int *edges) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Graph_get( comm,  maxindex,  maxedges,  index,  edges);
	write_EVENT_PAIR(MPI_Graph_get_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Graph_map (MPI_Comm comm, int nnodes, int *index, int *edges, int *newrank) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Graph_map( comm,  nnodes,  index,  edges,  newrank);
	write_EVENT_PAIR(MPI_Graph_map_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Graph_neighbors_count (MPI_Comm comm, int rank, int *nneighbors) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Graph_neighbors_count( comm,  rank,  nneighbors);
	write_EVENT_PAIR(MPI_Graph_neighbors_count_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Graph_neighbors (MPI_Comm comm, int rank, int maxneighbors, int *neighbors) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Graph_neighbors( comm,  rank,  maxneighbors,  neighbors);
	write_EVENT_PAIR(MPI_Graph_neighbors_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Graphdims_get (MPI_Comm comm, int *nnodes, int *nedges) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Graphdims_get( comm,  nnodes,  nedges);
	write_EVENT_PAIR(MPI_Graphdims_get_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Group_compare (MPI_Group group1, MPI_Group group2, int *result) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Group_compare( group1,  group2,  result);
	write_EVENT_PAIR(MPI_Group_compare_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Group_difference (MPI_Group group1, MPI_Group group2,  MPI_Group *newgroup) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Group_difference( group1,  group2,   newgroup);
	write_EVENT_PAIR(MPI_Group_difference_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Group_excl (MPI_Group group, int n, int *ranks, MPI_Group *newgroup) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Group_excl( group,  n,  ranks,  newgroup);
	write_EVENT_PAIR(MPI_Group_excl_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Group_free (MPI_Group *group) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Group_free( group);
	write_EVENT_PAIR(MPI_Group_free_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Group_incl (MPI_Group group, int n, int *ranks, MPI_Group *newgroup) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Group_incl( group,  n,  ranks,  newgroup);
	write_EVENT_PAIR(MPI_Group_incl_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Group_intersection (MPI_Group group1, MPI_Group group2,  MPI_Group *newgroup) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Group_intersection( group1,  group2,   newgroup);
	write_EVENT_PAIR(MPI_Group_intersection_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Group_range_excl (MPI_Group group, int n, int ranges[][3],  MPI_Group *newgroup) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Group_range_excl( group,  n,  ranges  ,   newgroup);
	write_EVENT_PAIR(MPI_Group_range_excl_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Group_range_incl (MPI_Group group, int n, int ranges[][3],  MPI_Group *newgroup) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Group_range_incl( group,  n,  ranges  ,   newgroup);
	write_EVENT_PAIR(MPI_Group_range_incl_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Group_rank (MPI_Group group, int *rank) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Group_rank( group,  rank);
	write_EVENT_PAIR(MPI_Group_rank_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Group_size (MPI_Group group, int *size) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Group_size( group,  size);
	write_EVENT_PAIR(MPI_Group_size_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Group_translate_ranks (MPI_Group group1, int n, int *ranks1,   MPI_Group group2, int *ranks2) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Group_translate_ranks( group1,  n,  ranks1,    group2,  ranks2);
	write_EVENT_PAIR(MPI_Group_translate_ranks_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Group_union (MPI_Group group1, MPI_Group group2, MPI_Group *newgroup) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Group_union( group1,  group2,  newgroup);
	write_EVENT_PAIR(MPI_Group_union_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Ibsend (void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Ibsend( buf,  count,  datatype,  dest,  tag,  comm,  request);
	write_EVENT_PAIR(MPI_Ibsend_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Info_create (MPI_Info *info) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Info_create( info);
	write_EVENT_PAIR(MPI_Info_create_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Info_delete (MPI_Info info, char *key) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Info_delete( info,  key);
	write_EVENT_PAIR(MPI_Info_delete_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Info_dup (MPI_Info info, MPI_Info *newinfo) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Info_dup( info,  newinfo);
	write_EVENT_PAIR(MPI_Info_dup_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Info_free (MPI_Info *info) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Info_free( info);
	write_EVENT_PAIR(MPI_Info_free_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Info_get (MPI_Info info, char *key, int valuelen, char *value, int *flag) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Info_get( info,  key,  valuelen,  value,  flag);
	write_EVENT_PAIR(MPI_Info_get_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Info_get_nkeys (MPI_Info info, int *nkeys) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Info_get_nkeys( info,  nkeys);
	write_EVENT_PAIR(MPI_Info_get_nkeys_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Info_get_nthkey (MPI_Info info, int n, char *key) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Info_get_nthkey( info,  n,  key);
	write_EVENT_PAIR(MPI_Info_get_nthkey_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Info_get_valuelen (MPI_Info info, char *key, int *valuelen,  int *flag) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Info_get_valuelen( info,  key,  valuelen,   flag);
	write_EVENT_PAIR(MPI_Info_get_valuelen_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Info_set (MPI_Info info, char *key, char *value) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Info_set( info,  key,  value);
	write_EVENT_PAIR(MPI_Info_set_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Init_thread (int *argc, char ***argv, int required, int *provided) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Init_thread( argc,  argv,  required,  provided);
	write_EVENT_PAIR(MPI_Init_thread_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Intercomm_create (MPI_Comm local_comm, int local_leader,  MPI_Comm bridge_comm, int remote_leader,  int tag, MPI_Comm *newintercomm) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Intercomm_create( local_comm,  local_leader,   bridge_comm,  remote_leader,   tag,  newintercomm);
	write_EVENT_PAIR(MPI_Intercomm_create_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Intercomm_merge (MPI_Comm intercomm, int high, MPI_Comm *newintercomm) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Intercomm_merge( intercomm,  high,  newintercomm);
	write_EVENT_PAIR(MPI_Intercomm_merge_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Iprobe (int source, int tag, MPI_Comm comm, int *flag, MPI_Status *status) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Iprobe( source,  tag,  comm,  flag,  status);
	write_EVENT_PAIR(MPI_Iprobe_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Irecv (void *buf, int count, MPI_Datatype datatype, int source,   int tag, MPI_Comm comm, MPI_Request *request) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Irecv( buf,  count,  datatype,  source,    tag,  comm,  request);
	write_EVENT_PAIR(MPI_Irecv_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Irsend (void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Irsend( buf,  count,  datatype,  dest,  tag,  comm,  request);
	write_EVENT_PAIR(MPI_Irsend_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Isend (void *buf, int count, MPI_Datatype datatype, int dest,   int tag, MPI_Comm comm, MPI_Request *request) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Isend( buf,  count,  datatype,  dest,    tag,  comm,  request);
	write_EVENT_PAIR(MPI_Isend_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Issend (void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Issend( buf,  count,  datatype,  dest,  tag,  comm,  request);
	write_EVENT_PAIR(MPI_Issend_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Keyval_create (MPI_Copy_function *copy_fn, MPI_Delete_function *delete_fn, int *keyval, void *extra_state) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Keyval_create( copy_fn,  delete_fn,  keyval,  extra_state);
	write_EVENT_PAIR(MPI_Keyval_create_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Keyval_free (int *keyval) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Keyval_free( keyval);
	write_EVENT_PAIR(MPI_Keyval_free_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Op_create (MPI_User_function *function, int commute, MPI_Op *op) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Op_create( function,  commute,  op);
	write_EVENT_PAIR(MPI_Op_create_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Op_free (MPI_Op *op) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Op_free( op);
	write_EVENT_PAIR(MPI_Op_free_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Pack (void *inbuf, int incount, MPI_Datatype datatype,   void *outbuf, int outsize, int *position, MPI_Comm comm) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Pack( inbuf,  incount,  datatype,    outbuf,  outsize,  position,  comm);
	write_EVENT_PAIR(MPI_Pack_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Pack_size (int incount, MPI_Datatype datatype, MPI_Comm comm, int *size) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Pack_size( incount,  datatype,  comm,  size);
	write_EVENT_PAIR(MPI_Pack_size_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Probe (int source, int tag, MPI_Comm comm, MPI_Status *status) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Probe( source,  tag,  comm,  status);
	write_EVENT_PAIR(MPI_Probe_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Recv_init (void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Request *request) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Recv_init( buf,  count,  datatype,  source,  tag,  comm,  request);
	write_EVENT_PAIR(MPI_Recv_init_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Recv (void *buf, int count, MPI_Datatype datatype, int source,   int tag, MPI_Comm comm, MPI_Status *status) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Recv( buf,  count,  datatype,  source,    tag,  comm,  status);
	write_EVENT_PAIR(MPI_Recv_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Reduce (void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Reduce( sendbuf,  recvbuf,  count,  datatype,  op,  root,  comm);
	write_EVENT_PAIR(MPI_Reduce_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Reduce_scatter (void *sendbuf, void *recvbuf, int *recvcounts, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Reduce_scatter( sendbuf,  recvbuf,  recvcounts,  datatype,  op,  comm);
	write_EVENT_PAIR(MPI_Reduce_scatter_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Register_datarep (char *datarep,  MPI_Datarep_conversion_function *read_conversion_fn,  MPI_Datarep_conversion_function *write_conversion_fn,  MPI_Datarep_extent_function *dtype_file_extent_fn,  void *extra_state) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Register_datarep( datarep,   read_conversion_fn,   write_conversion_fn,   dtype_file_extent_fn,   extra_state);
	write_EVENT_PAIR(MPI_Register_datarep_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Request_free (MPI_Request *request) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Request_free( request);
	write_EVENT_PAIR(MPI_Request_free_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Rsend (void *ibuf, int count, MPI_Datatype datatype, int dest,   int tag, MPI_Comm comm) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Rsend( ibuf,  count,  datatype,  dest,    tag,  comm);
	write_EVENT_PAIR(MPI_Rsend_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Rsend_init (void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Rsend_init( buf,  count,  datatype,  dest,  tag,  comm,  request);
	write_EVENT_PAIR(MPI_Rsend_init_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Scan (void *sendbuf, void *recvbuf, int count,   MPI_Datatype datatype, MPI_Op op, MPI_Comm comm) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Scan( sendbuf,  recvbuf,  count,    datatype,  op,  comm);
	write_EVENT_PAIR(MPI_Scan_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Scatter (void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Scatter( sendbuf,  sendcount,  sendtype,  recvbuf,  recvcount,  recvtype,  root,  comm);
	write_EVENT_PAIR(MPI_Scatter_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Scatterv (void *sendbuf, int *sendcounts, int *displs, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Scatterv( sendbuf,  sendcounts,  displs,  sendtype,  recvbuf,  recvcount,  recvtype,  root,  comm);
	write_EVENT_PAIR(MPI_Scatterv_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Send_init (void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Send_init( buf,  count,  datatype,  dest,  tag,  comm,  request);
	write_EVENT_PAIR(MPI_Send_init_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Send (void *buf, int count, MPI_Datatype datatype, int dest,   int tag, MPI_Comm comm) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Send( buf,  count,  datatype,  dest,    tag,  comm);
	write_EVENT_PAIR(MPI_Send_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Sendrecv (void *sendbuf, int sendcount, MPI_Datatype sendtype, int dest, int sendtag, void *recvbuf, int recvcount, MPI_Datatype recvtype, int source, int recvtag, MPI_Comm comm, MPI_Status *status) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Sendrecv( sendbuf,  sendcount,  sendtype,  dest,  sendtag,  recvbuf,  recvcount,  recvtype,  source,  recvtag,  comm,  status);
	write_EVENT_PAIR(MPI_Sendrecv_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Sendrecv_replace (void * buf, int count, MPI_Datatype datatype,  int dest, int sendtag, int source, int recvtag,  MPI_Comm comm, MPI_Status *status) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Sendrecv_replace(  buf,  count,  datatype,   dest,  sendtag,  source,  recvtag,   comm,  status);
	write_EVENT_PAIR(MPI_Sendrecv_replace_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Ssend_init (void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Ssend_init( buf,  count,  datatype,  dest,  tag,  comm,  request);
	write_EVENT_PAIR(MPI_Ssend_init_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Ssend (void *buf, int count, MPI_Datatype datatype, int dest,   int tag, MPI_Comm comm) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Ssend( buf,  count,  datatype,  dest,    tag,  comm);
	write_EVENT_PAIR(MPI_Ssend_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Start (MPI_Request *request) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Start( request);
	write_EVENT_PAIR(MPI_Start_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Startall (int count, MPI_Request *array_of_requests) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Startall( count,  array_of_requests);
	write_EVENT_PAIR(MPI_Startall_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Status_set_cancelled (MPI_Status *status, int flag) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Status_set_cancelled( status,  flag);
	write_EVENT_PAIR(MPI_Status_set_cancelled_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Status_set_elements (MPI_Status *status, MPI_Datatype datatype,  int count) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Status_set_elements( status,  datatype,   count);
	write_EVENT_PAIR(MPI_Status_set_elements_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Testall (int count, MPI_Request array_of_requests[], int *flag, MPI_Status array_of_statuses[]) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Testall( count,  array_of_requests ,  flag,  array_of_statuses );
	write_EVENT_PAIR(MPI_Testall_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Testany (int count, MPI_Request array_of_requests[], int *index, int *flag, MPI_Status *status) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Testany( count,  array_of_requests ,  index,  flag,  status);
	write_EVENT_PAIR(MPI_Testany_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Test (MPI_Request *request, int *flag, MPI_Status *status) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Test( request,  flag,  status);
	write_EVENT_PAIR(MPI_Test_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Test_cancelled (MPI_Status *status, int *flag) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Test_cancelled( status,  flag);
	write_EVENT_PAIR(MPI_Test_cancelled_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Testsome (int incount, MPI_Request array_of_requests[], int *outcount, int array_of_indices[], MPI_Status array_of_statuses[]) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Testsome( incount,  array_of_requests ,  outcount,  array_of_indices ,  array_of_statuses );
	write_EVENT_PAIR(MPI_Testsome_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Topo_test (MPI_Comm comm, int *status) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Topo_test( comm,  status);
	write_EVENT_PAIR(MPI_Topo_test_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Waitall (int count, MPI_Request *array_of_requests, MPI_Status *array_of_statuses) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Waitall( count,  array_of_requests,  array_of_statuses);
	write_EVENT_PAIR(MPI_Waitall_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Waitany (int count, MPI_Request *array_of_requests, int *index, MPI_Status *status) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Waitany( count,  array_of_requests,  index,  status);
	write_EVENT_PAIR(MPI_Waitany_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Wait (MPI_Request *request, MPI_Status *status) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Wait( request,  status);
	write_EVENT_PAIR(MPI_Wait_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Waitsome (int incount, MPI_Request *array_of_requests, int *outcount, int *array_of_indices, MPI_Status *array_of_statuses) {
	write_END_PROCESSING();
	long startTime = time_us();
	int ret = PMPI_Waitsome( incount,  array_of_requests,  outcount,  array_of_indices,  array_of_statuses);
	write_EVENT_PAIR(MPI_Waitsome_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


double MPI_Wtick (void) {
	write_END_PROCESSING();
	long startTime = time_us();
	double ret = PMPI_Wtick( );
	write_EVENT_PAIR(MPI_Wtick_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}


double MPI_Wtime (void) {
	write_END_PROCESSING();
	long startTime = time_us();
	double ret = PMPI_Wtime( );
	write_EVENT_PAIR(MPI_Wtime_event, startTime);
	write_BEGIN_PROCESSING();
	return ret;
}





// Automatically generated by generate_pmpi_wrappers.pl
// This file contains 169 wrapped mpi calls.
// PMPI_ProjectionsLogging.C wraps up two others (MPI_Init, MPI_Finalize)
