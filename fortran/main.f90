program main
  use io_function
  use update_function
  implicit none

  type(particle),allocatable::particles(:,:,:)
  real*8::alpha
  integer::time_steps, output_freq, args
  character(100)::buffer
  character(:),allocatable::result_name, input_name

  allocate(particles(0:1,0:1,0:1))
  
  args = NARGS()
  if(args /= 4) then
    print *, "Incorrect arguments"
    print *, "Please call the program in the following format"
    print *, "  ./myproject input.dat output_freq output"
    call exit(1)
  end if
  call GETARG(1, buffer)
  allocate(character(len(trim(buffer)))::input_name)
  input_name = trim(buffer)
  call GETARG(2, buffer)
  read(buffer, *) output_freq
  call GETARG(3, buffer)
  allocate(character(len(trim(buffer)))::result_name)
  result_name = trim(buffer)
  call read_particle_file(particles, input_name, alpha, time_steps)
  call simulation(particles, result_name, time_steps, output_freq, alpha)

  deallocate(result_name)
  deallocate(input_name)
  deallocate(particles)
end program
