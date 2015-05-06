module io_function
  implicit none

  type::particle
    real*8:: u
    integer:: hold
  end type

  contains

  subroutine read_particle_file(particles, pfile_name, alpha, time_steps)
    type(particle),allocatable,intent(inout)::particles(:,:,:)
    character(:),allocatable::pfile_name
    character(10)::x, y
    real*8,intent(inout)::alpha
    integer::xdim, ydim, h, iostatus, i, j, xint, yint, time_steps
    real*8::temperature

    open(unit=2, FILE=pfile_name, action="read")
    read(2,*) xdim, ydim, alpha, time_steps
    deallocate(particles) !just to make interface consistent, the subroutine takes in allocated array, returns allocated array
    allocate(particles(0:(xdim - 1), 0:(ydim - 1), 0:2))
    read(2,*) x, y, temperature, h
    do while(iostatus == 0)
      if(x == "*") then
        if(y == "*") then!two wild cards
          do j=0, ydim-1
            do i=0, xdim-1
              particles(i,j,0)%u = temperature
              particles(i,j,0)%hold = h
            end do
          end do
        else !only x is wild card
          do i=0, xdim-1
            read(y,*) yint
            particles(i, yint, 0)%u = temperature
            particles(i, yint, 0)%hold = h
          end do
        end if
      else if(y == "*") then !only y is wild card
        read(x,*) xint
        do j=0, ydim-1
          particles(xint, j, 0)%u = temperature
          particles(xint, j, 0)%hold = h
        end do
      else
        read(x,*) xint
        read(y,*) yint
        particles(xint, yint, 0)%u = temperature
        particles(xint, yint, 0)%hold = h
      end if
      read(2,*,iostat=iostatus) x, y, temperature, h
    end do
    close(2)
  end subroutine

  subroutine write_result_file(particles, time, outfile_name)
    integer::time, i, j
    character(:),allocatable::final_outfile_name, outfile_name
    type(particle)::particles(0:, 0:, 0:)
    
    allocate(character(len(outfile_name) + len("_00000.dat"))::final_outfile_name)
    write(final_outfile_name, '(2a,I5.5,a)'), outfile_name, "_", time, ".dat"
    open(unit=2, FILE=final_outfile_name, action="write")
    do i=0, size(particles, 1)-1
      do j=0, size(particles, 2)-1
        write(2, *) i, j, particles(i,j,Mod(time,2))%u
      end do
      write(2,*) ""
    end do
    deallocate(final_outfile_name)
    close(2)
  end subroutine
end module
