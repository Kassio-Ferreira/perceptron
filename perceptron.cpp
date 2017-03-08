#include <RcppArmadillo.h>
// [[Rcpp::depends(RcppArmadillo)]]

using namespace Rcpp;
using namespace arma;

// [[Rcpp::export]]
rowvec perceptronC(mat x, vec y, float eta, int niter) {
  
  rowvec weight;
  rowvec errors;
  int tam = x.n_cols+1;
  double z;
  int ypred;
  rowvec weightdiff;
  
  weight.zeros(tam);
  errors.zeros(niter);
  
  for(int jj=0; jj<niter; jj++){
    for(int ii=0; ii<y.size(); ii++){
      
      z = sum(weight.subvec(1,weight.size()-1) % x.row(ii)) + weight(0);
      
      if(z < 0){
        ypred = -1;
      } else{
        ypred = 1;
      }
      
      rowvec temp = ones<rowvec>(1);
      //temp.ones(1);
      
      weightdiff = eta * (y(ii)-ypred) * join_rows(temp,x.row(ii));
      weight = weight + weightdiff;
      
      if((y(ii)-ypred) != 0){
        errors[jj] += 1;  
      }
      
    }
    
  }
  
  return (errors);
}

/*** R
library(microbenchmark)

# load iris data set
data(iris)
  
# subset of iris data frame - extract only species versicolor and setosa
# we will only focus on the sepal and petal lengths of the dataset
  irissubdf <- iris[1:100, c(1, 3, 5)]
names(irissubdf) <- c("sepal", "petal", "species")
  head(irissubdf)
  
  irissubdf[, 4] <- 1
irissubdf[irissubdf[, 3] == "setosa", 4] <- -1

x <- irissubdf[, c(1, 2)]
y <- irissubdf[, 4]

# head and tail of data 
#head(x)
  
  
perceptron <- function(x, y, eta, niter) {
    
# initialize weight vector
    weight <- rep(0, dim(x)[2] + 1)
    errors <- rep(0, niter)
    
    
# loop over number of epochs niter
    for (jj in 1:niter) {
      
# loop through training data set
      for (ii in 1:length(y)) {
        
# Predict binary label using Heaviside activation 
# function
        z <- sum(weight[2:length(weight)] * 
          as.numeric(x[ii, ])) + weight[1]
        if(z < 0) {
          ypred <- -1
        } else {
          ypred <- 1
        }
        
# Change weight - the formula doesn't do anything 
# if the predicted value is correct
        weightdiff <- eta * (y[ii] - ypred) * 
          c(1, as.numeric(x[ii, ]))
          weight <- weight + weightdiff
          
# Update error function
          if ((y[ii] - ypred) != 0.0) {
            errors[jj] <- errors[jj] + 1
          }
          
      }
      #print(weight)
    }
    
# weight to decide between the two species 
    
    return(errors)
  }

# Compare de outputs
perceptronC(as.matrix(x),as.vector(y),1,10)
perceptron(x,y,1,10)

# let's see who won
microbenchmark(
UsingC = perceptronC(as.matrix(x),as.vector(y),1,10),
UsingR = perceptron(x,y,1,10)
)

*/
