#############################################################################
#  This file is part of the Stride software. 
#  It is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by 
#  the Free Software Foundation, either version 3 of the License, or any 
#  later version.
#  The software is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#  You should have received a copy of the GNU General Public License,
#  along with the software. If not, see <http://www.gnu.org/licenses/>.
#  see http://www.gnu.org/licenses/.
#
#
#  Copyright 2018, Willem L, Kuylen E & Broeckhove J
#############################################################################
#
# MODEL SUMMARY EXPLORATION
# - input-output behavior
# - transmission events and context
#
#############################################################################

explore_summary <- function(project_dir)
{
  
  # check if project_dir exists
  if(.rstride$dir_not_present(project_dir)){
    return(-1)
  }
  
  # load project output summary
  project_summary    <- .rstride$load_project_summary(project_dir)
  
  # retrieve all variable model parameters
  input_opt_design   <- .rstride$get_variable_model_param(project_summary)
  
  # continue only if there are different input parameter values
  if(nrow(input_opt_design)>1){
    
    # calculate a scale factor and ticks for the second y-axis [cases vs. incidence]
    range_num_cases   <- range(project_summary$num_cases)
    ticks_cases       <- seq(range_num_cases[1],range_num_cases[2],diff(range_num_cases)/5) 
    
    r0_axis_factor     <- median(project_summary$num_cases / project_summary$AR)
    r0_axis_scale      <- diff(range_num_cases)/r0_axis_factor
  
    num_digits        <- ceiling(abs(log10(r0_axis_scale)))
    ticks_r0          <- round(ticks_cases/r0_axis_factor,digits=num_digits)
    
    # OPEN PDF STREAM
    pdf(file.path(project_dir,'parameter_exploration.pdf'))
    
    # loop over the changing input parameters => plot cases and incidence
    #par(mfrow=c(2,2))
    par(mar = c(5, 4, 4, 4) + 0.3)  # Leave space for 3rd axis
    for(i in 1:ncol(input_opt_design)){
      boxplot(num_cases ~ project_summary[,colnames(input_opt_design)[i]],
              data = project_summary,
              xlab = colnames(input_opt_design)[i],
              ylab = '')
      axis(4, at = ticks_cases , labels = ticks_r0 )
      mtext("incidence", side=4, line=2,cex=0.9)
      mtext("number of cases", side=2, line=2,cex=0.9)
    }
    
    dev.off()
  } # end if(length(input_opt)>0)
  
  
  # terminal message
  .rstride$cli_print('SUMMARY EXPLORATION COMPLETE')
  
}

## HELP FUNCTION
.rstride$get_variable_model_param <- function(project_summary){
  
  col_output <- c('run_time', 'total_time', 'num_cases', 'AR' )
  col_extra  <- c('rng_seed','output_prefix','transmission_rate','exp_id') 
  col_input  <- !(names(project_summary) %in% c(col_output,col_extra))
  
  input_opt    <- lapply(project_summary[,col_input],unique)
  input_opt    <- input_opt[lapply(input_opt,length)>1]
  
  # get parameter combinations
  input_opt_design <- unique(project_summary[,names(input_opt)])
  
  # with only one parameter, convert vector into matrix
  if(length(input_opt)==1){
    input_opt_design <- as.matrix(data.frame(input_opt))
  }
  
  # with identical parameters, use the r0
  if(length(input_opt)==0){
    input_opt_design <- as.matrix(data.frame(r0=unique(project_summary$r0)))
  }
  
  return(input_opt_design)

}