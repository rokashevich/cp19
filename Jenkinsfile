pipeline {
  agent any
  stages {
    stage('Build') {
      steps {
        sh '''
        docker run --rm \
        --mount type=bind,source=`pwd`,target=/w \
        rokashevich/cp19-amd64-builder /w/build.sh
        '''
      }
    }
  }
}
