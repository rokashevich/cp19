pipeline {
  agent any
  stages {
    stage('Build') {
      steps {
        sh -c '''
        docker run --rm -it \
        --mount type=bind,source=`pwd`,target=/w \
        rokashevich/cp19-amd64-builder /w/build.sh
        '''
      }
    }
  }
}
