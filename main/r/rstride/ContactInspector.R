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

# Load packages
library(ggplot2,quietly = TRUE)
library(grid,quietly = TRUE)
library(gridExtra,quietly = TRUE)

# load help functions
source('./bin/rstride/misc.R')

if(0==1) # for debugging
{
  #setwd('..')
  exp_summary <- project_summary[1,]
  data_dir    <- './data'
  plot_contacts(exp_summary,data_dir)
}

#############################################################################
# FUNCTION TO PLOT SOCIAL CONTACT MATRICES AND COUNTS                      ##
#############################################################################
plot_contacts <- function(exp_summary,data_dir)
{
  
  ######################
  ## GET DATA       ##
  ######################
  
  # load data
  load(file.path(exp_summary$output_prefix,'data_contacts.RData'))
  load(file.path(exp_summary$output_prefix,'data_participants.RData'))
  

  if(dim(data_cnt)[1]>0 && dim(data_part)[1]>0)
  {
    
    ## people without contacts
    dim(data_part)[1] - length(unique(data_cnt$local_id))
    
    ## employed and student population
    data_part$employed <- data_part$workplace_id != 0
    data_part$student  <- data_part$school_id != 0
    
    ## SETTINGS 
    L <- 80
    num_days      <- exp_summary$num_days
    
    pdf(paste0(exp_summary$output_prefix,'_cnt_patterns.pdf'),10,5)
    #par(mfrow=c(2,2))
    ## TOTAL
    mij_total  <- .rstride$plot_cnt_matrix(data_cnt,data_part,'total',L,num_days)
    
    ## HOUSEHOLD
    mij_hh     <- .rstride$plot_cnt_matrix(data_cnt[data_cnt$cnt_home==1,],data_part,'household',L,num_days)
    
    ## SCHOOL
    mij_school <- .rstride$plot_cnt_matrix(data_cnt[data_cnt$cnt_school==1,],data_part[data_part$student==T,],'school',L,num_days)
    
    ## WORK
    mij_work   <- .rstride$plot_cnt_matrix(data_cnt[data_cnt$cnt_work==1,],data_part[data_part$employed==T,],'work',L,num_days)
    
    ## PRIMARY COMMUNITY
    mij_prim_comm <- .rstride$plot_cnt_matrix(data_cnt[data_cnt$cnt_prim_comm==1,],data_part,'prim_comm',L,num_days)
    
    ## SECUNDARY COMMUNITY
    mij_sec_comm <- .rstride$plot_cnt_matrix(data_cnt[data_cnt$cnt_sec_comm==1,],data_part,'sec_comm',L,num_days)
    
    #dev.off()
    
    ref_data_tag <- 'ref_fl2010'
    if(grepl('15touch',exp_summary$age_contact_matrix_file)){
      ref_data_tag <- 'ref_fl2010_15touch'
    }
    
    # LOAD SURVEY DATA FROM FLANDERS AND FULLY CONNECTED HOUSEHOLDS
    #survey_mij_hh         <- read.table(file=paste0(project_dir,'/data/ref_miami_household_gam_mij_rec.csv'),sep=';',dec=',',header=T)
    survey_mij_hh         <- read.table(file=file.path(data_dir,paste0(ref_data_tag,'_regular_weekday_household_gam_mij_rec.csv')),sep=';',dec=',',header=T)
    survey_mij_school     <- read.table(file=file.path(data_dir,paste0(ref_data_tag,'_regular_weekday_school_student_age24_gam_mij_median.csv')),sep=';',dec=',',header=T)
    survey_mij_work       <- read.table(file=file.path(data_dir,paste0(ref_data_tag,'_regular_weekday_workplace_employed_age_gam_mij_median.csv')),sep=';',dec=',',header=T)
    survey_mij_community  <- read.table(file=file.path(data_dir,paste0(ref_data_tag,'_regular_weekday_community_gam_mij_rec.csv')),sep=';',dec=',',header=T)
    survey_mij_total      <- read.table(file=file.path(data_dir,paste0(ref_data_tag,'_regular_weekday_gam_mij_rec.csv')),sep=';',dec=',',header=T)
    
    survey_mij_school_weekend     <- survey_mij_school*0
    survey_mij_work_weekend       <- survey_mij_work*0
    survey_mij_community_weekend  <- read.table(file=file.path(data_dir,paste0(ref_data_tag,'_weekend_community_gam_mij_rec.csv')),sep=';',dec=',',header=T)
    survey_mij_total_weekend      <- read.table(file=file.path(data_dir,paste0(ref_data_tag,'_weekend_gam_mij_rec.csv')),sep=';',dec=',',header=T)
    
    
    ## COMPARE
    par(mfrow=c(2,3))
    
    plot(rowSums(survey_mij_total),main='total',xlab='age',ylab='contacts',type='l',ylim=c(-0.1,30))
    lines(rowSums(survey_mij_total_weekend),main='total',xlab='age',ylab='contacts',type='l',lty=2)
    points(rowSums(mij_total),col=2)
    legend('topright',c('week','weekend','model'),col=c(1,1,2),lty=c(1,2,0),pch=c(-1,-1,1),cex=0.8,title=ref_data_tag)
    
    plot(rowSums(survey_mij_hh),main='household',xlab='age',ylab='contacts',type='l')
    points(rowSums(mij_hh),col=2)
    legend('topright',c('week','weekend','model'),col=c(1,1,2),lty=c(1,2,0),pch=c(-1,-1,1),cex=0.8,title=ref_data_tag)
    
    plot(rowSums(survey_mij_school),main='school',xlab='age',ylab='contacts',type='l',ylim=c(-0.1,20))
    lines(rowSums(survey_mij_school_weekend),type='l',lty=2)
    points(rowSums(mij_school),col=2)
    legend('topright',c('week','weekend','model'),col=c(1,1,2),lty=c(1,2,0),pch=c(-1,-1,1),cex=0.8,title=ref_data_tag)
    
    plot(rowSums(survey_mij_work),main='work',xlab='age',ylab='contacts',type='l',ylim=c(-0.1,20))
    lines(rowSums(survey_mij_work_weekend),type='l',lty=2)
    points(rowSums(mij_work),col=2)
    legend('topright',c('week','weekend','model'),col=c(1,1,2),lty=c(1,2,0),pch=c(-1,-1,1),cex=0.8,title=ref_data_tag)
    
    plot(rowSums(survey_mij_community),main='primary community',xlab='age',ylab='contacts',type='l',ylim=c(-0.1,25))
    lines(rowSums(survey_mij_community_weekend),type='l',lty=2)
    points(rowSums(mij_prim_comm),col=2)
    legend('topright',c('week','weekend','model'),col=c(1,1,2),lty=c(1,2,0),pch=c(-1,-1,1),cex=0.8,title=ref_data_tag)
    
    plot(rowSums(survey_mij_community),main='secondary community',xlab='age',ylab='contacts',type='l',ylim=c(-0.1,25))
    lines(rowSums(survey_mij_community_weekend),type='l',lty=2)
    points(rowSums(mij_sec_comm),col=2)
    legend('topright',c('week','weekend','model'),col=c(1,1,2),lty=c(1,2,0),pch=c(-1,-1,1),cex=0.8,title=ref_data_tag)
    par(mfrow=c(1,1))
    
    dev.off() # close pdf stream
    
    # terminal message
    .rstride$cli_print('SOCIAL CONTACTS PLOTS COMPLETE FOR', exp_summary$output_prefix)

  } # end if dim(data)...
} # end function

#################################  EMBEDDED HELP FUNCTIONS  #################################

## HELP FUNCTION: RESHAPE DATA AND PLOT
.rstride$plot_cnt_matrix <- function(f_data_cnt,f_data_part,tag,L,num_days)
{
  
  # select participants
  data_cnt_flag <- f_data_cnt$local_id %in% f_data_part$local_id 
  
  
  # temporary max age
  L_temp <- max(f_data_cnt$part_age,L,f_data_cnt$cnt_age)+1
  
  # count contacts
  mij_tbl <- table(f_data_cnt$part_age,f_data_cnt$cnt_age)
  row_ind <- as.numeric(row.names(mij_tbl)) +1 # age 0 == index 1
  col_ind <- as.numeric(colnames(mij_tbl))  +1 # age 0 == index 1
  mij <- matrix(0,L_temp,L_temp)
  mij[row_ind,col_ind] <- mij_tbl
  
  # count participant per age
  if(sum(mij)==0)
  {
    row_ind <- 1:(L+1)
    col_ind <- 1:(L+1)
  }
  p_ages_tbl              <- table(f_data_part$part_age)
  
  row_ind <- as.numeric(names(p_ages_tbl)) +1
  p_ages <- matrix(0,L_temp,1)
  p_ages[row_ind] <- p_ages_tbl
  
  # remove age>L (age_column L+1)
  mij <- mij[1:(L+1),1:(L+1)]
  p_ages <- p_ages[1:(L+1)]
  
  # adjust for number of participants (if age is present)
  ages_present <- p_ages>0
  p_ages[ages_present]
  
  # contacts/participant
  for(j in 1:(L+1))
  {
    if(p_ages[j]>0)
    {
      mij[j,] <- mij[j,] / p_ages[j]
    }
  }
  
  # account for multiple days
  mij <- mij/num_days
  
  # plot matrix 
  g_matrix <- .rstride$plot_cnt_matrix_ggplot(mij,tag,FALSE)
  
  # plot number of contacts
  g_count  <- .rstride$plot_cnt_count_ggplot(f_data_cnt,f_data_part,L,num_days,tag)
  
  grid.arrange(g_matrix, g_count, ncol = 2)
  
  return(mij)
}

## HELP FUNCTION: PLOT CNT MATRIX
.rstride$plot_cnt_matrix_ggplot <- function(mij,title,bool_contour)
{
  ## remove small numbers
  mij[mij < quantile(mij,0.1)] <- 0
  
  # Function to rescale z according to quantiles
  mij_ecdf <- ecdf(mij)
  
  # Covert matrix into data.frame for plotting with ggplot
  ggplot_data <- expand.grid(x = 0:(nrow(mij) - 1), y = 0:(nrow(mij) - 1))
  ggplot_data <- within(ggplot_data, {
    z <- as.vector(mij)
    z.rescaled <- mij_ecdf(mij)
  })
  
  
  z.breaks <- signif(unique(quantile(ggplot_data$z, prob = seq(from = 0, to = 1, length = 5))), digits = 1)
  z.breaks.rescaled <- mij_ecdf(z.breaks)
  
  # Create the plot
  g <- ggplot(data = ggplot_data, mapping = aes(x = x, y = y, fill = z.rescaled, z = z.rescaled)) +
    geom_raster() +
    scale_fill_distiller(
      palette = "YlOrRd",
      breaks = z.breaks.rescaled, labels = z.breaks,
      name = 'Rate') +
    labs(x = "Age", y = "Age of contacts") +
    scale_x_continuous(expand = c(0, 0)) +
    scale_y_continuous(expand = c(0, 0)) +
    coord_fixed() +
    theme_bw() +
    ggtitle(title) +
    theme(legend.justification = c(1, 1),
          legend.position = 'right',
          legend.text = element_text(size=18),
          legend.title = element_text(size=18),
          axis.text=element_text(size=20),
          axis.title=element_text(size=20),
          plot.title = element_text(size=40, face="bold",hjust = 0.5)
    )
  
  # Add contour lines?
  if (bool_contour) {
    g + geom_contour(breaks = z.breaks, colour = "black", size = 0.2)
  } 
  
  # return
  return(g)
}

## HELP FUNCTION: PLOT CNT COUNT
.rstride$plot_cnt_count_ggplot <- function(f_data_cnt,f_data_part,L,num_days,title){
  
  if(nrow(f_data_cnt)==0){
    ggplot_data <-data.frame(local_id = -1,
                             part_age = f_data_part$part_age,
                             cnt_count = 0)
  } else{
    # Covert matrix into data.frame for plotting with ggplot
    ggplot_data        <- data.frame(table(f_data_cnt$local_id)/ num_days) 
    names(ggplot_data) <- c('local_id','cnt_count')
    ggplot_data <- merge(ggplot_data,f_data_part)
  }
  
  # remove oldest ages
  ggplot_data <- ggplot_data[ggplot_data$part_age<=L,]
  
  # average count per age
  cnt_age_mean <- aggregate(.~ part_age , data = ggplot_data[,2:3] ,mean) 
  
  # create plot
  g_plot <- ggplot(ggplot_data, aes(x=part_age, y=cnt_count)) + 
    aes(group = part_age) +
    geom_boxplot() +
    labs(x = "Age", y = "Count") +
    theme_bw() +
    ggtitle(title) +
    geom_line(data = cnt_age_mean, aes(x=part_age, y=cnt_count,group = 1),
              size=2, colour="red") +
    theme(legend.justification = c(1, 1),
          legend.position = 'right',
          legend.text = element_text(size=18),
          legend.title = element_text(size=18),
          axis.text=element_text(size=20),
          axis.title=element_text(size=20),
          plot.title = element_text(size=40, face="bold",hjust = 0.5)
    )
  return(g_plot)
  
}
