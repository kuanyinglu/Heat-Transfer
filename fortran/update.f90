module update_function
  use io_function
  implicit none

  contains
  
  type(particle) function update(particlei, x, y, particles, alpha, array_num) !both particlei and particles are from last time point
    type(particle)::particlei
    type(particle)::particles(0:, 0:, 0:)
    real*8:: alpha
    integer::xdim, ydim, x, y, array_num
    xdim = size(particles, 1)
    ydim = size(particles, 2)
    
    if(particlei%hold == 1) then !no update needed
      update%hold = 1
      update%u = particlei%u
    else
      update%hold = 0
      if(x == 0) then
        if(y == 0) then !top left corner
          update%u = particlei%u + alpha*(-3*particles(x,y,array_num)%u + particles(x+1,y,array_num)%u + particles(x+1,y+1,array_num)%u + particles(x,y+1,array_num)%u)
        else if(y == ydim-1) then !bottom left corner
          update%u = particlei%u + alpha*(-3*particles(x,y,array_num)%u + particles(x+1,y,array_num)%u + particles(x+1,y-1,array_num)%u + particles(x,y-1,array_num)%u)
        else !left edge
          update%u = particlei%u + alpha*(-5*particles(x,y,array_num)%u + particles(x,y-1,array_num)%u + particles(x,y+1,array_num)%u + particles(x+1,y-1,array_num)%u + particles(x+1,y,array_num)%u + particles(x+1,y+1,array_num)%u)
        end if
      else if(x == xdim-1) then
        if(y == 0) then !top right corner
          update%u = particlei%u + alpha*(-3*particles(x,y,array_num)%u + particles(x-1,y,array_num)%u + particles(x-1,y+1,array_num)%u + particles(x,y+1,array_num)%u)
        else if(y == ydim-1) then !bottom right corner
          update%u = particlei%u + alpha*(-3*particles(x,y,array_num)%u + particles(x-1,y,array_num)%u + particles(x-1,y-1,array_num)%u + particles(x,y-1,array_num)%u)
        else !right edge
          update%u = particlei%u + alpha*(-5*particles(x,y,array_num)%u + particles(x,y-1,array_num)%u + particles(x,y+1,array_num)%u + particles(x-1,y-1,array_num)%u + particles(x-1,y,array_num)%u + particles(x-1,y+1,array_num)%u)
        end if
      else if(y ==0) then !top edge
          update%u = particlei%u + alpha*(-5*particles(x,y,array_num)%u + particles(x-1,y,array_num)%u + particles(x+1,y,array_num)%u + particles(x-1,y+1,array_num)%u + particles(x,y+1,array_num)%u + particles(x+1,y+1,array_num)%u)
      else if(y == ydim-1) then !bottom edge
          update%u = particlei%u + alpha*(-5*particles(x,y,array_num)%u + particles(x-1,y,array_num)%u + particles(x+1,y,array_num)%u + particles(x-1,y-1,array_num)%u + particles(x,y-1,array_num)%u + particles(x+1,y-1,array_num)%u)
      else !middle
          update%u = particlei%u + alpha*(-8*particles(x,y,array_num)%u + particles(x-1,y-1,array_num)%u + particles(x-1,y,array_num)%u + particles(x-1,y+1,array_num)%u + particles(x,y-1,array_num)%u + particles(x,y+1,array_num)%u + particles(x+1,y-1,array_num)%u + particles(x+1,y,array_num)%u + particles(x+1,y+1,array_num)%u)
      end if
    end if
  end function

  subroutine simulation(particles, result_name, time_step, out_freq, alpha)
    type(particle)::particles(0:, 0:, 0:)
    integer::time_step, out_freq, t, i, j
    real*8::alpha
    character(:),allocatable::result_name

    call write_result_file(particles, 0, result_name)
    do t=1, time_step
      do i=0, size(particles,1)-1
        do j=0, size(particles,2)-1
          particles(i,j,Mod(t,2)) = update(particles(i,j,Mod(t-1,2)), i, j, particles, alpha, Mod(t-1,2))
        end do
      end do
      if(Mod(t, out_freq) == 0) then
        call write_result_file(particles, t, result_name)
      end if
    end do
  end subroutine
end module
