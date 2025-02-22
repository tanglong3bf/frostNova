import { useCommonStore } from '@/stores/common'
import axios, { type AxiosResponse } from 'axios'
import { storeToRefs } from 'pinia'
import { ElMessage } from 'element-plus'
import router from '@/router'

interface BaseResponse {
  code?: number
  data?: any
}

interface SuccessResponse extends BaseResponse {
  message: string
  error?: never
}

interface ErrorResponse extends BaseResponse {
  error: string
  message?: never
}
type Response = SuccessResponse | ErrorResponse

type NullableResponse = Response | null

const instance = axios.create({
  baseURL: 'http://localhost:8000',
  timeout: 5000
})

instance.interceptors.request.use(
  config => {
    const commonStore = useCommonStore()
    const { token } = storeToRefs(commonStore)
    if (token.value !== '') {
      // 每次请求时添加 token
      config.headers['Authorization'] = 'Bearer ' + token.value
    }
    return config
  },
  error => {
    ElMessage.error(error || 'Error')
    return Promise.reject(error)
  }
)

instance.interceptors.response.use(
  (response: AxiosResponse<NullableResponse>) => {
    if (response.status === 204) {
      return null
    }
    const { code, message, error, data } = response.data || {}
    // 如果 code 小于 0，则表示发生了错误
    if (code !== undefined && code < 0) {
      console.log('code: ', code)
      ElMessage.error(error || 'Error')
      return Promise.reject(new Error(error || 'Error'))
    } else {
      if (message !== undefined) {
        ElMessage.success(message)
      }
      return data
    }
  },
  error => {
    console.log('error: ', error)
    console.log('error.response: ', error.response)
    console.log('error.response.data: ', error.response.data)
    if (error.status === 401) {
      // 未登录
      const code = error.response.data?.code || -1
      let message
      if (code >= 3000 && code < 4000) {
        message = "请您先登录"
      } else {
        message = error.response.data?.error || "请您先登录"
      }
      ElMessage.error(message)
      console.log('Attempting to redirect to login page')
      router.push('/login')
      console.log(router)
    }
    else {
      ElMessage.error(error.response.data?.error || 'Error')
    }
    return Promise.reject(error)
  }
)

export default instance
