#include <iostream>
#include <vector>

int partition(int arr[],int lowIndex, int highIndex){

  int pivotValue = arr[highIndex]; //pivot comparison is the last element
  int pivotIndex = lowIndex;
  
  for(int i = lowIndex; i < highIndex ; i++){
    if(arr[i] < pivotValue){
      std::swap(arr[i],arr[pivotIndex]);
      pivotIndex++;
    }
  }
  std::swap(arr[pivotIndex], arr[highIndex]);

  return pivotIndex;
  
}


void quickSort(int arr[], int lowIndex,int highIndex){
  if(lowIndex < highIndex){
    
    
    int pivotPoint = partition(arr, lowIndex, highIndex);

    quickSort(arr,lowIndex,pivotPoint - 1);
    quickSort(arr,pivotPoint + 1,highIndex);
    
  }
}

void merge(int arr[],int lowIndex,int midIndex, int highIndex ){

  int lSize = midIndex - lowIndex + 1;
  int rSize = highIndex - midIndex;
  std::vector<int> lArr(lSize);
  std::vector<int> rArr(rSize);

  for(int i = 0; i < lSize; i++){
    lArr[i] = arr[lowIndex + i];
  }
  for(int j = 0; j < rSize; j++){
    rArr[j] = arr[midIndex +j +1];
  }

  int i = 0;
  int j = 0;
  int k = lowIndex;

  while(i < lSize && j < rSize){
    if(lArr[i] < rArr[j]){
      arr[k] = lArr[i];
      i++;
    } else{
      arr[k] = rArr[j];
      j++;
    }
    k++;
  }

  while(i < lSize){
    arr[k] = lArr[i];
    i++;
    k++;
  }
  while(j < rSize){
    arr[k] = rArr[j];
    j++;
    k++;
  }
  
}

void mergeSort(int arr[], int lowIndex, int highIndex){

  if(lowIndex < highIndex){

    int mid = lowIndex + (highIndex - lowIndex)/2;
    

    mergeSort(arr,lowIndex,mid);
    mergeSort(arr, mid + 1, highIndex);
    merge(arr, lowIndex, mid, highIndex);
    
  }
  


}











int main(){
  int arr[] = {2,-1,1,3,0,5};
  int arrSize = sizeof(arr)/sizeof(arr[0]);

  //quickSort(arr, 0, arrSize - 1);
  mergeSort(arr,0,arrSize - 1);

  for(int i = 0; i < arrSize; i++){
    std::cout << arr[i];
  }
  
  

}
