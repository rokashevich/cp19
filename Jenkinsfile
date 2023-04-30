pipeline {
  stages {
    stage('Build') {
      steps {
        docker run --rm -it --mount type=bind,source=`pwd`,target=/w rokashevich/cp19-amd64-builder /w/build.sh
      }
    }
  }
}
