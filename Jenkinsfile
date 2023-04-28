pipeline {
    agent {
        docker { image 'rokashevich/cp19_amd64_builder' }
    }
    stages {
        stage('Test') {
            steps {
                sh 'clang --version'
            }
        }
    }
}
