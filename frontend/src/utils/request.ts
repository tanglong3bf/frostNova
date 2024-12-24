import { useCommonStore } from '@/stores/common'
import axios, { type AxiosResponse } from 'axios'
import { storeToRefs } from 'pinia'
import { ElMessage } from 'element-plus'

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
      config.headers['Authorization'] = 'Bearer '+ token.value
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
      ElMessage.success(message || 'Success')
      return data
     }
  },
  error => {
    return Promise.reject(error)
  }
)

export default instance