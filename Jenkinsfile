pipeline {
  agent any
  stages {
    stage('Build') {
      steps {
        bash "docker run --rm -it --mount type=bind,source=$(pwd),target=/w rokashevich/cp19-amd64-builder /w/build.sh"
      }
    }
  }
}
